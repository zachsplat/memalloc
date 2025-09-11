#ifndef POOL_H
#define POOL_H

#include <stddef.h>

/*
 * fixed-size pool allocator. pre-allocates a big chunk
 * and hands out fixed-size blocks from it.
 *
 * free list is embedded in the free blocks themselves
 * (each free block's first 8 bytes point to next free block)
 */

struct pool {
    void *mem;          /* backing memory */
    void *free_list;    /* head of free list */
    size_t obj_size;    /* size of each object (>= 8) */
    size_t capacity;    /* total number of objects */
    size_t used;
};

int pool_init(struct pool *p, size_t obj_size, size_t count);
void *pool_alloc(struct pool *p);
void pool_free(struct pool *p, void *ptr);
void pool_destroy(struct pool *p);

/* stats */
size_t pool_used(struct pool *p);
size_t pool_available(struct pool *p);

#endif
