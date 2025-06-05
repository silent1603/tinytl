#ifndef ARENA_H
#define ARENA_H

#include "tinytl/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct Arena {
    uint8_t *base;
    size_t capacity;
    size_t offset;
} Arena;


Arena arena_create(size_t capacity) {
    Arena arena;
    arena.base = (uint8_t*)malloc(capacity);
    arena.capacity = capacity;
    arena.offset = 0;
    assert(arena.base && "Arena allocation failed!");
    return arena;
}


void* arena_alloc(Arena *arena, size_t size) {
    if (arena->offset + size > arena->capacity) {
        fprintf(stderr, "Arena out of memory!\n");
        return NULL;
    }
    void *ptr = arena->base + arena->offset;
    arena->offset += size;
    return ptr;
}


void arena_reset(Arena *arena) {
    arena->offset = 0;
}


void arena_destroy(Arena *arena) {
    free(arena->base);
    arena->base = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}
#endif