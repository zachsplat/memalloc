CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11 -g
AR = ar

SRC = src/pool.c src/slab.c
OBJ = $(SRC:.c=.o)
LIB = libmemalloc.a

all: $(LIB)

$(LIB): $(OBJ)
	$(AR) rcs $@ $^

test_alloc: test/test_main.c $(LIB)
	$(CC) $(CFLAGS) -o $@ $< -L. -lmemalloc -Iinclude

test: test_alloc
	./test_alloc

clean:
	rm -f $(OBJ) $(LIB) test_alloc

.PHONY: all test clean
