/*
 * mm.c - A simple boundary-tag allocator for malloclab.
 *
 * This implementation uses an implicit free list, first-fit placement,
 * block splitting, and immediate boundary-tag coalescing. Each heap
 * block has a 4-byte header and footer storing its size and allocation
 * bit. The heap starts with an aligned prologue block and ends with an
 * epilogue header.
 *
 * This version is intentionally kept close to the textbook reference so
 * it can serve as a readable baseline implementation.
 */
#include <stdio.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "ChenJiaCheng",
    /* Second member's email address (leave blank if none) */
    "2311087@mail.nankai.edu.cn"
};

/* Basic constants and macros */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12)
#define MIN_BLOCK_SIZE (2 * DSIZE)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p) (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p) (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

static char *heap_listp = NULL;

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static int check_block(void *bp);

/*
 * mm_check - Validate basic heap invariants for debugging.
 */
int mm_check(void)
{
    char *bp;

    if (heap_listp == NULL) {
        return 1;
    }

    if (GET_SIZE(HDRP(heap_listp)) != DSIZE || !GET_ALLOC(HDRP(heap_listp))) {
        return 0;
    }

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!check_block(bp)) {
            return 0;
        }

        if (!GET_ALLOC(HDRP(bp)) && !GET_ALLOC(HDRP(NEXT_BLKP(bp)))) {
            return 0;
        }
    }

    if (GET_SIZE(HDRP(bp)) != 0 || !GET_ALLOC(HDRP(bp))) {
        return 0;
    }

    return 1;
}

/*
 * mm_init - Initialize the heap with prologue and epilogue blocks.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1) {
        return -1;
    }

    PUT(heap_listp, 0);
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));
    heap_listp += (2 * WSIZE);

    if (extend_heap(CHUNKSIZE / WSIZE) == NULL) {
        return -1;
    }

    return 0;
}

/*
 * mm_malloc - Allocate a block with at least size bytes of payload.
 */
void *mm_malloc(size_t size)
{
    size_t asize;
    size_t extendsize;
    void *bp;

    if (size == 0) {
        return NULL;
    }

    if (heap_listp == NULL && mm_init() == -1) {
        return NULL;
    }

    if (size <= DSIZE) {
        asize = MIN_BLOCK_SIZE;
    } else {
        asize = DSIZE * ((size + DSIZE + (DSIZE - 1)) / DSIZE);
    }

    bp = find_fit(asize);
    if (bp != NULL) {
        place(bp, asize);
        return bp;
    }

    extendsize = MAX(asize, CHUNKSIZE);
    bp = extend_heap(extendsize / WSIZE);
    if (bp == NULL) {
        return NULL;
    }

    place(bp, asize);
    return bp;
}

/*
 * mm_free - Mark a block as free and coalesce adjacent free blocks.
 */
void mm_free(void *ptr)
{
    size_t size;

    if (ptr == NULL) {
        return;
    }

    size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr);
}

/*
 * mm_realloc - Reallocate a block by allocate-copy-free.
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *newptr;
    size_t old_payload_size;
    size_t copy_size;

    if (ptr == NULL) {
        return mm_malloc(size);
    }

    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    newptr = mm_malloc(size);
    if (newptr == NULL) {
        return NULL;
    }

    old_payload_size = GET_SIZE(HDRP(ptr)) - DSIZE;
    copy_size = MIN(size, old_payload_size);
    memcpy(newptr, ptr, copy_size);
    mm_free(ptr);
    return newptr;
}

/*
 * extend_heap - Extend the heap and create a new free block.
 */
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    bp = mem_sbrk((int)size);
    if (bp == (void *)-1) {
        return NULL;
    }

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return coalesce(bp);
}

/*
 * coalesce - Merge adjacent free blocks and return the merged block.
 */
static void *coalesce(void *bp)
{
    size_t prev_alloc;
    size_t next_alloc;
    size_t size;

    prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {
        return bp;
    }

    if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        return bp;
    }

    if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        return PREV_BLKP(bp);
    }

    size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
    PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
    PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
    return PREV_BLKP(bp);
}

/*
 * find_fit - First-fit search over the implicit free list.
 */
static void *find_fit(size_t asize)
{
    char *bp;

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp)) {
        if (!GET_ALLOC(HDRP(bp)) && GET_SIZE(HDRP(bp)) >= asize) {
            return bp;
        }
    }

    return NULL;
}

/*
 * place - Mark a free block allocated and split if the remainder is large enough.
 */
static void place(void *bp, size_t asize)
{
    size_t csize;

    csize = GET_SIZE(HDRP(bp));
    if ((csize - asize) >= MIN_BLOCK_SIZE) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize - asize, 0));
        PUT(FTRP(bp), PACK(csize - asize, 0));
        return;
    }

    PUT(HDRP(bp), PACK(csize, 1));
    PUT(FTRP(bp), PACK(csize, 1));
}

/*
 * check_block - Verify per-block alignment and header/footer consistency.
 */
static int check_block(void *bp)
{
    if (((size_t)bp % DSIZE) != 0) {
        return 0;
    }

    if (GET(HDRP(bp)) != GET(FTRP(bp))) {
        return 0;
    }

    return 1;
}













