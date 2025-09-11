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
