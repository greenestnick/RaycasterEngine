#ifndef RENDERLIST
#define RENDERLIST

#include <stdlib.h>
#include <stdint.h>

typedef struct{
    float xRay;
    float yRay;
    float xRayLength;
    float yRayLength;
    Uint32 xTile;
    Uint32 yTile;
    Uint32 column;
    Uint8 steppingInX;
}RayHit;

typedef struct{
    uint32_t size;
    uint32_t head;
    RayHit* list_array;
}RenderList;

RenderList ListInit(uint32_t startingSize){
    if(startingSize == 0) startingSize = 1;
    RayHit* arrayAddr = (RayHit*)calloc(startingSize, sizeof(RayHit));
    return (RenderList){startingSize, 0, arrayAddr};
}

void ListAppend(RenderList* list, RayHit item){
    if(list->head < list->size){
        list->list_array[list->head] = item;
        list->head++;
        return;
    }
    
    list->size += list->size >> 2;
    list->list_array = (RayHit*)realloc(list->list_array, sizeof(RayHit) * list->size);
    list->list_array[list->head] = item;
    list->head++;
}

void ListClear(RenderList* list, uint32_t newSize){
    list->head = 0;
    list->size = newSize;
    list->list_array = (RayHit*)realloc(list->list_array, sizeof(RayHit)*newSize);
}

void ListReturnHome(RenderList* list){
    list->head = 0;
}

void ListDestroy(RenderList* list){
    free(list->list_array);
    list->list_array = NULL;
}

#endif