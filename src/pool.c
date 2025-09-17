#include "pool.h"
#include <stdlib.h>
#include <string.h>

int pool_init(struct pool *p, size_t obj_size, size_t count)
{
    /* minimum 8 bytes so we can store free list pointer */
    if (obj_size < sizeof(void *))
        obj_size = sizeof(void *);

    /* align to 8 bytes */
    obj_size = (obj_size + 7) & ~(size_t)7;

    p->mem = malloc(obj_size * count);
    if (!p->mem) return -1;

    p->obj_size = obj_size;
    p->capacity = count;
    p->used = 0;

    /* build free list */
    p->free_list = p->mem;
    char *ptr = (char *)p->mem;
    for (size_t i = 0; i < count - 1; i++) {
        *(void **)(ptr + i * obj_size) = ptr + (i + 1) * obj_size;
    }
    *(void **)(ptr + (count - 1) * obj_size) = NULL;

    return 0;
}

void *pool_alloc(struct pool *p)
{
    if (!p->free_list) return NULL;

    void *block = p->free_list;
    p->free_list = *(void **)block;
    p->used++;
    return block;
}

void pool_free(struct pool *p, void *ptr)
{
    if (!ptr) return;

    /* stick it back on the free list */
    *(void **)ptr = p->free_list;
    p->free_list = ptr;
    p->used--;
}

void pool_destroy(struct pool *p)
{
    free(p->mem);
    p->mem = NULL;
    p->free_list = NULL;
}

size_t pool_used(struct pool *p) { return p->used; }
size_t pool_available(struct pool *p) { return p->capacity - p->used; }

int pool_init_aligned(struct pool *p, size_t obj_size, size_t count, size_t alignment)
{
    if (obj_size < sizeof(void *))
        obj_size = sizeof(void *);

    /* round up to alignment */
    obj_size = (obj_size + alignment - 1) & ~(alignment - 1);

    /* use aligned_alloc */
    p->mem = aligned_alloc(alignment, obj_size * count);
    if (!p->mem) return -1;

    p->obj_size = obj_size;
    p->capacity = count;
    p->used = 0;

    p->free_list = p->mem;
    char *ptr = (char *)p->mem;
    for (size_t i = 0; i < count - 1; i++) {
        *(void **)(ptr + i * obj_size) = ptr + (i + 1) * obj_size;
    }
    *(void **)(ptr + (count - 1) * obj_size) = NULL;

    return 0;
}
