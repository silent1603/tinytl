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
    Arena arena = {
        .capacity = capacity,
        .size = 0,
        .data = 0
    };
    return arena;
}


void* arena_alloc(Arena *arena, size_t size) {
    if (arena->data + size > arena->capacity) {
        fprintf(stderr, "Arena out of memory!\n");
        return NULL;
    }
    void *ptr = arena->data + arena->size;
    arena->size += size;
    return ptr;
}


void arena_reset(Arena *arena) {
    arena->size = 0;
}


void arena_destroy(Arena *arena) {
    free(arena->data);
    arena->data = NULL;
    arena->capacity = 0;
    arena->size = 0;
}
#endif