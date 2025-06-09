#ifndef ARENA_H
#define ARENA_H

#include "tinytl/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

typedef struct {
    uint8_t *data;
    size_t capacity;
    size_t size;
} Arena;


Arena arena_init(size_t capacity) {
    void* data = malloc(sizeof(uint8_t) * capacity);
    Arena arena = {
        .capacity = capacity,
        .size = 0,
        .data = data
    };
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