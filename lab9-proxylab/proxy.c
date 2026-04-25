#include "csapp.h"
#include <strings.h>

#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400
#define CACHE_ENTRY_COUNT 16

static const char *user_agent_hdr =
    "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

typedef struct {
    char key[MAXLINE];
    char *data;
    size_t size;
    unsigned long long stamp;
    int valid;
    pthread_rwlock_t lock;
} cache_entry_t;

static cache_entry_t cache[CACHE_ENTRY_COUNT];
static pthread_mutex_t cache_mutex = PTHREAD_MUTEX_INITIALIZER;
static size_t cache_bytes = 0;
static unsigned long long cache_clock = 0;

static ssize_t write_all(int fd, const void *buf, size_t len)
{
    size_t left = len;
    const char *ptr = buf;

    while (left > 0) {
        ssize_t written = write(fd, ptr, left);

        if (written < 0) {
            if (errno == EINTR) {
                continue;
            }
            return -1;
        }
        if (written == 0) {
            return -1;
        }
        left -= (size_t)written;
        ptr += written;
    }

    return (ssize_t)len;
}

static void send_error_response(int clientfd, const char *errnum,
                                const char *shortmsg, const char *longmsg)
{
    char body[MAXBUF];
    char header[MAXBUF];
    int body_len;
    int header_len;

    body_len = snprintf(body, sizeof(body),
                        "<html><title>Proxy Error</title>"
                        "<body>%s: %s<p>%s</p></body></html>",
                        errnum, shortmsg, longmsg);
    if (body_len < 0 || (size_t)body_len >= sizeof(body)) {
        return;
    }

    header_len = snprintf(header, sizeof(header),
                          "HTTP/1.0 %s %s\r\n"
                          "Content-Type: text/html\r\n"
                          "Content-Length: %d\r\n"
                          "Connection: close\r\n\r\n",
                          errnum, shortmsg, body_len);
    if (header_len < 0 || (size_t)header_len >= sizeof(header)) {
        return;
    }

    write_all(clientfd, header, (size_t)header_len);
    write_all(clientfd, body, (size_t)body_len);
}

static void trim_trailing(char *text)
{
    size_t len = strlen(text);

    while (len > 0 &&
           (text[len - 1] == '\r' || text[len - 1] == '\n' ||
            isspace((unsigned char)text[len - 1]))) {
        text[--len] = '\0';
    }
}

static void parse_host_value(const char *value, char *host, size_t host_cap,
                             char *port, size_t port_cap)
{
    char temp[MAXLINE];
    char *ptr;
    char *colon;

    if (snprintf(temp, sizeof(temp), "%s", value) >= (int)sizeof(temp)) {
        host[0] = '\0';
        return;
    }

    ptr = temp;
    while (*ptr && isspace((unsigned char)*ptr)) {
        ptr++;
    }
    trim_trailing(ptr);
    if (*ptr == '\0') {
        host[0] = '\0';
        return;
    }

    colon = strrchr(ptr, ':');
    if (colon != NULL && strchr(colon + 1, ':') == NULL) {
        *colon = '\0';
        if (*(colon + 1) != '\0') {
            snprintf(port, port_cap, "%s", colon + 1);
        }
    }

    snprintf(host, host_cap, "%s", ptr);
}

static int parse_uri(const char *uri, char *host, size_t host_cap, char *port,
                     size_t port_cap, char *path, size_t path_cap)
{
    const char *start = uri;
    const char *slash;
    char authority[MAXLINE];
    size_t auth_len;

    host[0] = '\0';
    path[0] = '\0';
    snprintf(port, port_cap, "80");

    if (!strncasecmp(uri, "http://", 7)) {
        start = uri + 7;
    }

    if (*start == '/') {
        if (snprintf(path, path_cap, "%s", start) >= (int)path_cap) {
            return -1;
        }
        return 0;
    }

    slash = strchr(start, '/');
    if (slash != NULL) {
        auth_len = (size_t)(slash - start);
        if (auth_len >= sizeof(authority)) {
            return -1;
        }
        memcpy(authority, start, auth_len);
        authority[auth_len] = '\0';
        if (snprintf(path, path_cap, "%s", slash) >= (int)path_cap) {
            return -1;
        }
    } else {
        if (snprintf(authority, sizeof(authority), "%s", start) >=
            (int)sizeof(authority)) {
            return -1;
        }
        if (snprintf(path, path_cap, "/") >= (int)path_cap) {
            return -1;
        }
    }

    parse_host_value(authority, host, host_cap, port, port_cap);
    return host[0] == '\0' ? -1 : 0;
}

static int append_text(char *buf, size_t cap, size_t *used, const char *text)
{
    size_t len = strlen(text);

    if (*used + len + 1 > cap) {
        return -1;
    }

    memcpy(buf + *used, text, len + 1);
    *used += len;
    return 0;
}

static int append_format(char *buf, size_t cap, size_t *used,
                         const char *fmt, ...)
{
    va_list args;
    int written;

    va_start(args, fmt);
    written = vsnprintf(buf + *used, cap - *used, fmt, args);
    va_end(args);

    if (written < 0 || *used + (size_t)written >= cap) {
        return -1;
    }

    *used += (size_t)written;
    return 0;
}

static void build_cache_key(const char *host, const char *port, const char *path,
                            char *key, size_t cap)
{
    if (!strcmp(port, "80")) {
        snprintf(key, cap, "%s%s", host, path);
    } else {
        snprintf(key, cap, "%s:%s%s", host, port, path);
    }
}

static void cache_init(void)
{
    int i;

    for (i = 0; i < CACHE_ENTRY_COUNT; i++) {
        cache[i].key[0] = '\0';
        cache[i].data = NULL;
        cache[i].size = 0;
        cache[i].stamp = 0;
        cache[i].valid = 0;
        pthread_rwlock_init(&cache[i].lock, NULL);
    }
}

static int cache_find_locked(const char *key)
{
    int i;

    for (i = 0; i < CACHE_ENTRY_COUNT; i++) {
        if (cache[i].valid && !strcmp(cache[i].key, key)) {
            return i;
        }
    }
    return -1;
}

static int cache_find_free_locked(void)
{
    int i;

    for (i = 0; i < CACHE_ENTRY_COUNT; i++) {
        if (!cache[i].valid) {
            return i;
        }
    }
    return -1;
}

static int cache_find_victim_locked(void)
{
    int i;
    int victim = -1;
    unsigned long long oldest = 0;

    for (i = 0; i < CACHE_ENTRY_COUNT; i++) {
        if (!cache[i].valid) {
            continue;
        }
        if (victim < 0 || cache[i].stamp < oldest) {
            victim = i;
            oldest = cache[i].stamp;
        }
    }
    return victim;
}

static void cache_evict_locked(int idx)
{
    pthread_rwlock_wrlock(&cache[idx].lock);
    if (cache[idx].valid) {
        cache_bytes -= cache[idx].size;
        free(cache[idx].data);
        cache[idx].data = NULL;
        cache[idx].size = 0;
        cache[idx].stamp = 0;
        cache[idx].valid = 0;
        cache[idx].key[0] = '\0';
    }
    pthread_rwlock_unlock(&cache[idx].lock);
}

static int cache_send(const char *key, int clientfd)
{
    int idx;
    int status = 0;

    pthread_mutex_lock(&cache_mutex);
    idx = cache_find_locked(key);
    if (idx >= 0) {
        cache[idx].stamp = ++cache_clock;
        pthread_rwlock_rdlock(&cache[idx].lock);
    }
    pthread_mutex_unlock(&cache_mutex);

    if (idx < 0) {
        return 0;
    }

    if (write_all(clientfd, cache[idx].data, cache[idx].size) >= 0) {
        status = 1;
    } else {
        status = 1;
    }
    pthread_rwlock_unlock(&cache[idx].lock);
    return status;
}

static void cache_store(const char *key, const char *data, size_t size)
{
    char *copy;
    int idx;

    if (size == 0 || size > MAX_OBJECT_SIZE) {
        return;
    }

    copy = malloc(size);
    if (copy == NULL) {
        return;
    }
    memcpy(copy, data, size);

    pthread_mutex_lock(&cache_mutex);
    if (cache_find_locked(key) >= 0) {
        pthread_mutex_unlock(&cache_mutex);
        free(copy);
        return;
    }

    while (cache_bytes + size > MAX_CACHE_SIZE || cache_find_free_locked() < 0) {
        int victim = cache_find_victim_locked();

        if (victim < 0) {
            break;
        }
        cache_evict_locked(victim);
    }

    idx = cache_find_free_locked();
    if (idx < 0 || cache_bytes + size > MAX_CACHE_SIZE) {
        pthread_mutex_unlock(&cache_mutex);
        free(copy);
        return;
    }

    pthread_rwlock_wrlock(&cache[idx].lock);
    snprintf(cache[idx].key, sizeof(cache[idx].key), "%s", key);
    cache[idx].data = copy;
    cache[idx].size = size;
    cache[idx].stamp = ++cache_clock;
    cache[idx].valid = 1;
    cache_bytes += size;
    pthread_rwlock_unlock(&cache[idx].lock);
    pthread_mutex_unlock(&cache_mutex);
}

static void forward_response(int serverfd, int clientfd, const char *cache_key)
{
    rio_t server_rio;
    char buf[MAXBUF];
    char object_buf[MAX_OBJECT_SIZE];
    ssize_t n;
    size_t object_size = 0;
    int cacheable = 1;
    int client_failed = 0;

    rio_readinitb(&server_rio, serverfd);
    while ((n = rio_readnb(&server_rio, buf, sizeof(buf))) > 0) {
        if (!client_failed && write_all(clientfd, buf, (size_t)n) < 0) {
            client_failed = 1;
        }
        if (cacheable) {
            if (object_size + (size_t)n <= MAX_OBJECT_SIZE) {
                memcpy(object_buf + object_size, buf, (size_t)n);
                object_size += (size_t)n;
            } else {
                cacheable = 0;
            }
        }
    }

    if (n == 0 && cacheable) {
        cache_store(cache_key, object_buf, object_size);
    }
}

static void handle_client(int clientfd)
{
    rio_t client_rio;
    char line[MAXLINE];
    char method[MAXLINE];
    char uri[MAXLINE];
    char version[MAXLINE];
    char host[MAXLINE];
    char port[16];
    char path[MAXLINE];
    char host_header[MAXLINE];
    char other_headers[MAXLINE * 8];
    char request_buf[MAXLINE * 8];
    char cache_key[MAXLINE];
    size_t other_used = 0;
    size_t request_used = 0;
    ssize_t n;
    int serverfd;
    size_t host_len;
    size_t port_len;

    host[0] = '\0';
    path[0] = '\0';
    host_header[0] = '\0';
    other_headers[0] = '\0';
    request_buf[0] = '\0';
    snprintf(port, sizeof(port), "80");

    rio_readinitb(&client_rio, clientfd);
    n = rio_readlineb(&client_rio, line, sizeof(line));
    if (n <= 0) {
        return;
    }

    if (sscanf(line, "%s %s %s", method, uri, version) != 3) {
        send_error_response(clientfd, "400", "Bad Request",
                            "Proxy could not parse request line");
        return;
    }

    if (strcasecmp(method, "GET")) {
        send_error_response(clientfd, "501", "Not Implemented",
                            "Proxy only implements GET");
        return;
    }

    if (parse_uri(uri, host, sizeof(host), port, sizeof(port), path,
                  sizeof(path)) < 0) {
        send_error_response(clientfd, "400", "Bad Request",
                            "Proxy could not parse URI");
        return;
    }

    while ((n = rio_readlineb(&client_rio, line, sizeof(line))) > 0) {
        if (!strcmp(line, "\r\n")) {
            break;
        }
        if (!strncasecmp(line, "Host:", 5)) {
            snprintf(host_header, sizeof(host_header), "%s", line);
            if (host[0] == '\0') {
                parse_host_value(line + 5, host, sizeof(host), port,
                                 sizeof(port));
            }
            continue;
        }
        if (!strncasecmp(line, "User-Agent:", 11) ||
            !strncasecmp(line, "Connection:", 11) ||
            !strncasecmp(line, "Proxy-Connection:", 17)) {
            continue;
        }
        if (append_text(other_headers, sizeof(other_headers), &other_used, line) <
            0) {
            send_error_response(clientfd, "400", "Bad Request",
                                "Request headers are too large");
            return;
        }
    }

    if (host[0] == '\0') {
        send_error_response(clientfd, "400", "Bad Request",
                            "Proxy could not determine host");
        return;
    }

    if (path[0] == '\0') {
        snprintf(path, sizeof(path), "/");
    }

    if (host_header[0] == '\0') {
        host_len = strlen(host);
        port_len = strlen(port);
        if (!strcmp(port, "80")) {
            if (host_len + strlen("Host: \r\n") >= sizeof(host_header)) {
                send_error_response(clientfd, "400", "Bad Request",
                                    "Host header is too large");
                return;
            }
            memcpy(host_header, "Host: ", strlen("Host: "));
            memcpy(host_header + strlen("Host: "), host, host_len);
            memcpy(host_header + strlen("Host: ") + host_len, "\r\n", 3);
        } else {
            if (host_len + port_len + strlen("Host: :\r\n") >=
                sizeof(host_header)) {
                send_error_response(clientfd, "400", "Bad Request",
                                    "Host header is too large");
                return;
            }
            memcpy(host_header, "Host: ", strlen("Host: "));
            memcpy(host_header + strlen("Host: "), host, host_len);
            host_header[strlen("Host: ") + host_len] = ':';
            memcpy(host_header + strlen("Host: ") + host_len + 1, port,
                   port_len);
            memcpy(host_header + strlen("Host: ") + host_len + 1 + port_len,
                   "\r\n", 3);
        }
    }

    build_cache_key(host, port, path, cache_key, sizeof(cache_key));
    if (cache_send(cache_key, clientfd)) {
        return;
    }

    if (append_format(request_buf, sizeof(request_buf), &request_used,
                      "GET %s HTTP/1.0\r\n", path) < 0 ||
        append_text(request_buf, sizeof(request_buf), &request_used,
                    host_header) < 0 ||
        append_text(request_buf, sizeof(request_buf), &request_used,
                    user_agent_hdr) < 0 ||
        append_text(request_buf, sizeof(request_buf), &request_used,
                    "Connection: close\r\n") < 0 ||
        append_text(request_buf, sizeof(request_buf), &request_used,
                    "Proxy-Connection: close\r\n") < 0 ||
        append_text(request_buf, sizeof(request_buf), &request_used,
                    other_headers) < 0 ||
        append_text(request_buf, sizeof(request_buf), &request_used, "\r\n") <
            0) {
        send_error_response(clientfd, "400", "Bad Request",
                            "Proxy could not build outbound request");
        return;
    }

    serverfd = open_clientfd(host, port);
    if (serverfd < 0) {
        send_error_response(clientfd, "502", "Bad Gateway",
                            "Proxy could not connect to end server");
        return;
    }

    if (write_all(serverfd, request_buf, strlen(request_buf)) >= 0) {
        forward_response(serverfd, clientfd, cache_key);
    }
    close(serverfd);
}

static void *worker(void *vargp)
{
    int connfd = *((int *)vargp);

    free(vargp);
    Pthread_detach(Pthread_self());
    handle_client(connfd);
    close(connfd);
    return NULL;
}

int main(int argc, char **argv)
{
    int listenfd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    Signal(SIGPIPE, SIG_IGN);
    cache_init();

    listenfd = open_listenfd(argv[1]);
    if (listenfd < 0) {
        fprintf(stderr, "Failed to listen on port %s\n", argv[1]);
        return 1;
    }

    while (1) {
        struct sockaddr_storage clientaddr;
        socklen_t clientlen = sizeof(clientaddr);
        int *connfdp = malloc(sizeof(int));
        pthread_t tid;

        if (connfdp == NULL) {
            continue;
        }

        *connfdp = accept(listenfd, (SA *)&clientaddr, &clientlen);
        if (*connfdp < 0) {
            free(connfdp);
            if (errno == EINTR || errno == ECONNABORTED) {
                continue;
            }
            continue;
        }

        Pthread_create(&tid, NULL, worker, connfdp);
    }
}
