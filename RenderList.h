#ifndef RENDERLIST
#define RENDERLIST

#include <stdlib.h>
#include <stdint.h>
#include "stack.h" //TODO: TEMP, needs access to rayhit

typedef struct{
    uint32_t size;
    RayHit* list_array;
}RenderList;

RenderList ListInit(uint32_t startingSize){
    if(startingSize == 0) startingSize = 1;
    RayHit* arrayAddr = (RayHit*)calloc(startingSize, sizeof(RayHit));
    return (RenderList){startingSize, arrayAddr};
}

void ListAppend(RenderList* list, RayHit item){
    list->size++;
    list->list_array = (RayHit*)realloc(list->list_array, sizeof(RayHit)*list->size);
    list->list_array[list->size - 1] = item;
}

void ListClear(RenderList* list, uint32_t newSize){
    list->size = newSize;
    list->list_array = (RayHit*)realloc(list->list_array, sizeof(RayHit)*newSize);
}

void ListDestroy(RenderList* list){
    free(list->list_array);
    list->list_array = NULL;
}

#endif