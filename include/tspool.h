#ifndef TSPOOL_H
#define TSPOOL_H

/* thread-safe wrapper around pool allocator.
   just slaps a mutex on it, nothing clever. */

#include "pool.h"
#include <pthread.h>

struct tspool {
    struct pool pool;
    pthread_mutex_t lock;
};

static inline int tspool_init(struct tspool *tp, size_t obj_size, size_t count) {
    pthread_mutex_init(&tp->lock, NULL);
    return pool_init(&tp->pool, obj_size, count);
}

static inline void *tspool_alloc(struct tspool *tp) {
    pthread_mutex_lock(&tp->lock);
    void *p = pool_alloc(&tp->pool);
    pthread_mutex_unlock(&tp->lock);
    return p;
}

static inline void tspool_free(struct tspool *tp, void *ptr) {
    pthread_mutex_lock(&tp->lock);
    pool_free(&tp->pool, ptr);
    pthread_mutex_unlock(&tp->lock);
}

static inline void tspool_destroy(struct tspool *tp) {
    pool_destroy(&tp->pool);
    pthread_mutex_destroy(&tp->lock);
}

#endif
