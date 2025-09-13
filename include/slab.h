#ifndef SLAB_H
#define SLAB_H

#include "pool.h"

/*
 * slab allocator: multiple pools for different size classes.
 * sizes are power-of-2: 8, 16, 32, 64, 128, 256, 512, 1024
 *
 * anything bigger than 1024 falls through to malloc
 */

#define SLAB_NUM_CLASSES 8
#define SLAB_MAX_SIZE 1024

struct slab {
    struct pool pools[SLAB_NUM_CLASSES];
    size_t sizes[SLAB_NUM_CLASSES];
    int initialized;
};

int slab_init(struct slab *s, size_t objects_per_class);
void *slab_alloc(struct slab *s, size_t size);
void slab_free(struct slab *s, void *ptr, size_t size);  /* caller tracks size, yeah it sucks */
void slab_destroy(struct slab *s);

#endif
