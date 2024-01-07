#ifndef RENDERITEMLIST
#define RENDERITEMLIST

#include <stdlib.h>
#include <stdint.h>

typedef struct{
    float zDist;
    void* data;
}RenderItem;

typedef struct{
    uint32_t size;
    uint32_t head;
    RenderItem* list_array;
}RenderItemsList;

#define ListReturnHome(list) (list->head = 0)


RenderItemsList ListInit(uint32_t startingSize){
    if(startingSize == 0) startingSize = 1;
    RenderItem* arrayAddr = (RenderItem*)calloc(startingSize, sizeof(RenderItem));
    return (RenderItemsList){startingSize, 0, arrayAddr};
}

void ListAppend(RenderItemsList* list, RenderItem dist){
    if(list->size <= list->head){
        list->size += list->size >> 2;
        list->list_array = (RenderItem*)realloc(list->list_array, sizeof(RenderItem) * list->size);
    }
    
    list->list_array[list->head] = dist;
    list->head++;
}

void ListDestroy(RenderItemsList* list){
    free(list->list_array);
    list->list_array = NULL;
}

#endif