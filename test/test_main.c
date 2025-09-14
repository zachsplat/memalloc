#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../include/pool.h"
#include "../include/slab.h"

void test_pool(void)
{
    struct pool p;
    assert(pool_init(&p, 32, 100) == 0);
    assert(pool_available(&p) == 100);

    void *ptrs[100];
    for (int i = 0; i < 100; i++) {
        ptrs[i] = pool_alloc(&p);
        assert(ptrs[i] != NULL);
        memset(ptrs[i], 0xAA, 32);  /* scribble */
    }

    /* pool should be full */
    assert(pool_alloc(&p) == NULL);
    assert(pool_used(&p) == 100);

    /* free half */
    for (int i = 0; i < 50; i++) {
        pool_free(&p, ptrs[i]);
    }
    assert(pool_available(&p) == 50);

    /* allocate again */
    for (int i = 0; i < 50; i++) {
        void *q = pool_alloc(&p);
        assert(q != NULL);
    }

    pool_destroy(&p);
    printf("pool test ok\n");
}

void test_slab(void)
{
    struct slab s;
    assert(slab_init(&s, 256) == 0);

    /* small alloc */
    void *a = slab_alloc(&s, 10);
    assert(a != NULL);
    memset(a, 'x', 10);
    slab_free(&s, a, 10);

    /* various sizes */
    void *b = slab_alloc(&s, 100);
    void *c = slab_alloc(&s, 500);
    void *d = slab_alloc(&s, 1024);
    assert(b && c && d);

    /* big alloc falls through to malloc */
    void *e = slab_alloc(&s, 4096);
    assert(e != NULL);
    slab_free(&s, e, 4096);

    slab_free(&s, b, 100);
    slab_free(&s, c, 500);
    slab_free(&s, d, 1024);

    slab_destroy(&s);
    printf("slab test ok\n");
}

int main(void)
{
    test_pool();
    test_slab();
    printf("all passed\n");
    return 0;
}
