#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "cachelab.h"

struct cache_line{
    unsigned long addr;
    long LRU_time;
    int not_empty;
};

struct cache_sim{
    long S;
    long E;
    long B;
    long b;
    struct cache_line** data;
}cache;

long hits=0,misses=0,evictions=0;

void update_remain_LRU_time(long s_ind,long e_ind){
    for(long i=0;i<cache.S;i++){
        for(long j=0;j<cache.E;j++){
            if((i==s_ind && j<=e_ind)||cache.data[i][j].not_empty==0){continue;}
            cache.data[i][j].LRU_time++;
        }
    }
}

void load(unsigned long addr,char* status,int* status_ind){
    long LRU_max=-1;
    long LRU_e_ind=0;
    long s_ind=(addr>>cache.b)%cache.S;

    for(long e_ind=0;e_ind<cache.E;e_ind++){
        if(cache.data[s_ind][e_ind].not_empty==0){
            misses++;
            cache.data[s_ind][e_ind].addr=(addr>>cache.b)<<cache.b;
            cache.data[s_ind][e_ind].not_empty=1;
            strcpy(status, "miss ");
            *status_ind+=5;
            update_remain_LRU_time(s_ind,e_ind);
            break;
        }
        else if(cache.data[s_ind][e_ind].addr<=addr && (cache.data[s_ind][e_ind].addr+cache.B)>addr){
            hits++;
            cache.data[s_ind][e_ind].addr=(addr>>cache.b)<<cache.b;
            cache.data[s_ind][e_ind].LRU_time=0;
            strcpy(status, "hit ");
            *status_ind+=4;
            update_remain_LRU_time(s_ind,e_ind);
            break;
        }
        else{
            cache.data[s_ind][e_ind].LRU_time++;
            if(cache.data[s_ind][e_ind].LRU_time>=LRU_max){
                LRU_max=cache.data[s_ind][e_ind].LRU_time;
                LRU_e_ind=e_ind;
            }
            if(e_ind==cache.E-1){
                misses++;
                evictions++;
                strcpy(status, "miss eviction ");
                *status_ind+=14;
                
                cache.data[s_ind][LRU_e_ind].addr=addr;
                cache.data[s_ind][LRU_e_ind].LRU_time=0;
                update_remain_LRU_time(s_ind,e_ind);
            }
        }
    }
}


void store(unsigned long addr,char* status,int status_ind){
    long LRU_max=-1;
    long LRU_e_ind=0;
    long s_ind=(addr>>cache.b)%cache.S;

    for(long e_ind=0;e_ind<cache.E;e_ind++){
        if(cache.data[s_ind][e_ind].not_empty==0){
            misses++;
            cache.data[s_ind][e_ind].addr=(addr>>cache.b)<<cache.b;
            cache.data[s_ind][e_ind].not_empty=1;
            strcpy(status+status_ind, "miss");
            update_remain_LRU_time(s_ind,e_ind);
            break;
        }
        else if(cache.data[s_ind][e_ind].addr<=addr && cache.data[s_ind][e_ind].addr+cache.B>addr){
            hits++;
            cache.data[s_ind][e_ind].addr=(addr>>cache.b)<<cache.b;
            strcpy(status+status_ind, "hit");
            update_remain_LRU_time(s_ind,e_ind);
            break;
        }
        else{
            cache.data[s_ind][e_ind].LRU_time++;
            if(cache.data[s_ind][e_ind].LRU_time>LRU_max){
                LRU_max=cache.data[s_ind][e_ind].LRU_time;
                LRU_e_ind=e_ind;
            }
            if(e_ind==cache.E-1){
                misses++;
                evictions++;
                strcpy(status+status_ind, "miss eviction");
                
                cache.data[s_ind][LRU_e_ind].addr=(addr>>cache.b)<<cache.b;
                cache.data[s_ind][LRU_e_ind].LRU_time=0;
                update_remain_LRU_time(s_ind,e_ind);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    //cache 初始化工作
    const char *optstring = "s:E:b:t:v::";
    char *filename = NULL; 

    int option;
    int trace=0;
    while ((option = getopt(argc, argv, optstring)) != -1) {
        switch (option) {
            case 's':
                cache.S=1<<atoi(optarg);
                break;
            case 'E':
                cache.E=atoi(optarg);
                break;
            case 'b':
                cache.b=atoi(optarg);
                cache.B=1<<cache.b;
                break;
            case 't':
                filename = malloc(strlen(optarg) + 1);
                if (filename) 
                    strcpy(filename, optarg);
                break;
            case 'v':
                trace=1;
        }
    }
    cache.data=malloc(cache.S* sizeof(struct cache_line*));
    for(long i=0;i<cache.S;i++){
        cache.data[i]=calloc(cache.E,sizeof(struct cache_line));
    }

    //读取文件数据
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    char line[100];
    char op;
    unsigned long addr;
    int size;

    char status[30];

    while (fgets(line, sizeof(line), fp) != NULL) {
        // 跳过空行
        if (line[0] == '\n' || line[0] == '\0') 
            break;
        sscanf(line, " %c %lx,%d", &op, &addr, &size);
        if (op == 'I') 
            continue;

        int status_ind=0;
        switch(op){
            case 'L':
                load(addr,status,&status_ind);
                break;
            case 'S':
                store(addr,status,status_ind);
                break;
            case 'M':
                load(addr,status,&status_ind);
                store(addr,status,status_ind);
                break;
        }
        if(trace){
            printf("%c %lx,%d %s\n",op,addr,size,status);
        }
    }
    
    fclose(fp);

    //释放空间
    printSummary(hits, misses, evictions);
    for (int i = 0; i < cache.S; i++) {
        free(cache.data[i]); 
    }
    free(cache.data);
    free(filename);
    return 0;
}