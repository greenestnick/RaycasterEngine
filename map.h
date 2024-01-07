#ifndef MAP
#define MAP

#include <stdint.h>
#include <stdlib.h>
#include "TextureMap.h"

#define MakeDoor(depth, width, isXAligned, mapOriginAligned, isSolid) ((Door){depth, width, isXAligned, mapOriginAligned, isSolid})
#define MakeMultiWall(top, bottom, left, right) ((MultiWall){top, bottom, left, right})

typedef enum {WALL_NULL, WALL_TYPE, WALL_DOOR, WALL_MULTI} WallType;

typedef struct{
    float depth; //in positive x/y dir
    float width;
    uint8_t isXAligned;
    uint8_t mapOriginAligned;
    uint8_t isSolid;
}Door;

//TODO: Ensure no transparent texture is used for multiwalls
typedef struct{
  Texture top;
  Texture bottom;
  Texture left;
  Texture right;
}MultiWall;

typedef struct{
  WallType type;
  Texture tex;
  void* typeData;
}WallPiece;

typedef struct{
  Uint32 size;
  Uint32 levels; 
  WallPiece* mapData;
}MapStruct;


MapStruct Map_Init(Uint32 mapSize, Uint32 numLevels, TextureMap*const texMap, int*const mapArray){
  WallPiece* mapDataPtr = (WallPiece*)malloc(sizeof(WallPiece) * mapSize * mapSize * numLevels);
  for(Uint32 i = 0; i < mapSize * mapSize * numLevels; i++){
    mapDataPtr[i] = (WallPiece){(mapArray[i] > 0), mapArray[i], texMap, NULL};
  }

  return (MapStruct){mapSize, numLevels, mapDataPtr};
}

WallPiece* Map_GetWall(MapStruct*const map, Uint32 x, Uint32 y, Uint32 z){
  return map->mapData + x + map->size*y + (map->size*map->size)*z;
}

void Map_Destroy(MapStruct* const map){
  WallPiece* mapData = map->mapData;
  for(Uint32 i = 0; i < map->size * map->size; i++){
    if(mapData[i].typeData != NULL) free(mapData[i].typeData);
  }

  if(map->mapData != NULL) free(map->mapData);
  
}

void Map_AddDoor(MapStruct*const map, Uint32 x, Uint32 y, Door door){
  map->mapData[x + map->size * y].type = WALL_DOOR;
  map->mapData[x + map->size * y].typeData = (Door*)malloc(sizeof(Door));
  Door* doorPtr = (Door*) map->mapData[x + map->size * y].typeData;
  doorPtr->depth = door.depth;
  doorPtr->width = door.width;
  doorPtr->isXAligned = door.isXAligned;
  doorPtr->mapOriginAligned = door.mapOriginAligned;
  doorPtr->isSolid = door.isSolid;
}

void Map_AddMultiWall(MapStruct*const map, Uint32 x, Uint32 y, MultiWall wall){
  map->mapData[x + map->size * y].type = WALL_MULTI;
  map->mapData[x + map->size * y].typeData = (MultiWall**)malloc(sizeof(MultiWall));
  MultiWall* wallPtr = (MultiWall*) map->mapData[x + map->size * y].typeData;
  wallPtr->top = wall.top;
  wallPtr->bottom = wall.bottom;
  wallPtr->left = wall.left;
  wallPtr->right = wall.right;
}

#endif