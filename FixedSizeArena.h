#ifndef FIXEDSIZEARENA
#define FIXEDSIZEARENA

#include <stdint.h>
#include <stdlib.h>

typedef struct{
    uint32_t capacity; 
    uint32_t head;
    uint32_t count;
    uint8_t dataByteSize;
    uint8_t* arena_mem;
}Arena;

Arena Arena_Init(uint8_t dataByteSize, uint32_t startingCapacity){
    uint8_t* memptr = (uint8_t*)malloc(dataByteSize * startingCapacity);

    return (Arena){startingCapacity * dataByteSize, 0, 0, dataByteSize, memptr};
}

void* Arena_Append(Arena*const arena){
    if(arena->head == arena->capacity){
        arena->capacity = arena->capacity << 1; 
        arena->arena_mem = (uint8_t*)realloc(arena->arena_mem, arena->dataByteSize * arena->capacity);
    }

    void* memPtr = (void*)(arena->arena_mem + arena->head);
    arena->head += arena->dataByteSize;
    arena->count++;
    return memPtr;
}

void* Arena_Get(const Arena*const arena, uint32_t index){
    return (arena->arena_mem + arena->dataByteSize*index);
}

void Arena_Flush(Arena*const arena){
    arena->head = 0;
    arena->count = 0;
}

void Arena_Destroy(Arena*const arena){
    free(arena->arena_mem);
    arena->arena_mem = NULL;
    arena->capacity = 0;
    arena->head = 0;
    arena->count = 0;
}


#endif