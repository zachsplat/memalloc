#include "slab.h"
#include <stdlib.h>

static int size_class(size_t size)
{
    /* find which power-of-2 bucket */
    if (size <= 8)    return 0;
    if (size <= 16)   return 1;
    if (size <= 32)   return 2;
    if (size <= 64)   return 3;
    if (size <= 128)  return 4;
    if (size <= 256)  return 5;
    if (size <= 512)  return 6;
    if (size <= 1024) return 7;
    return -1;  /* too big */
}

int slab_init(struct slab *s, size_t objects_per_class)
{
    s->sizes[0] = 8;
    s->sizes[1] = 16;
    s->sizes[2] = 32;
    s->sizes[3] = 64;
    s->sizes[4] = 128;
    s->sizes[5] = 256;
    s->sizes[6] = 512;
    s->sizes[7] = 1024;

    for (int i = 0; i < SLAB_NUM_CLASSES; i++) {
        if (pool_init(&s->pools[i], s->sizes[i], objects_per_class) != 0)
            return -1;
    }
    s->initialized = 1;
    return 0;
}

void *slab_alloc(struct slab *s, size_t size)
{
    int cls = size_class(size);
    if (cls < 0) {
        /* fallback to malloc for big allocations */
        return malloc(size);
    }
    return pool_alloc(&s->pools[cls]);
}

void slab_free(struct slab *s, void *ptr, size_t size)
{
    int cls = size_class(size);
    if (cls < 0) {
        free(ptr);
        return;
    }
    pool_free(&s->pools[cls], ptr);
}

void slab_destroy(struct slab *s)
{
    if (!s->initialized) return;
    for (int i = 0; i < SLAB_NUM_CLASSES; i++) {
        pool_destroy(&s->pools[i]);
    }
    s->initialized = 0;
}
