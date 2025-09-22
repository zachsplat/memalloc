# memalloc

pool allocator for fixed-size objects. avoids malloc/free overhead
when you're allocating tons of small things.

also has a slab allocator for variable sizes (power-of-2 buckets).

nothing groundbreaking, just needed something like this for a project
and didn't want to pull in jemalloc for a small binary.

## build

```
make
make test
```

## api

```c
#include "pool.h"
#include "slab.h"

// pool: fixed-size blocks
struct pool p;
pool_init(&p, sizeof(my_struct), 1000);
my_struct *obj = pool_alloc(&p);
pool_free(&p, obj);

// slab: variable sizes (8 - 1024, power of 2 buckets)
struct slab s;
slab_init(&s, 512);  // 512 objects per size class
void *mem = slab_alloc(&s, 100);
slab_free(&s, mem, 100);  // have to pass size, I know
```

## thread safety

`tspool.h` wraps the pool with a mutex. not great for high contention
but fine for most uses. for real perf you'd want per-thread pools.
