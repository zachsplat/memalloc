#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../include/pool.h"

#define ITERS 1000000

int main(void)
{
    struct pool p;
    pool_init(&p, 64, ITERS);

    void **ptrs = malloc(sizeof(void *) * ITERS);

    clock_t start = clock();
    for (int i = 0; i < ITERS; i++)
        ptrs[i] = pool_alloc(&p);
    double alloc_time = (double)(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < ITERS; i++)
        pool_free(&p, ptrs[i]);
    double free_time = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("pool alloc: %d ops in %.4fs (%.0f ops/s)\n", ITERS, alloc_time, ITERS / alloc_time);
    printf("pool free:  %d ops in %.4fs (%.0f ops/s)\n", ITERS, free_time, ITERS / free_time);

    /* compare with malloc */
    start = clock();
    for (int i = 0; i < ITERS; i++)
        ptrs[i] = malloc(64);
    alloc_time = (double)(clock() - start) / CLOCKS_PER_SEC;

    start = clock();
    for (int i = 0; i < ITERS; i++)
        free(ptrs[i]);
    free_time = (double)(clock() - start) / CLOCKS_PER_SEC;

    printf("malloc:     %d ops in %.4fs (%.0f ops/s)\n", ITERS, alloc_time, ITERS / alloc_time);
    printf("free:       %d ops in %.4fs (%.0f ops/s)\n", ITERS, free_time, ITERS / free_time);

    free(ptrs);
    pool_destroy(&p);
    return 0;
}
