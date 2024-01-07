#ifndef RENDERITEM
#define RENDERITEM

#include <stdint.h>

typedef enum {RENDERITEM_SPRITE, RENDERITEM_WALL}RenderItemType;

typedef struct{
    float xCamPos;
    float height;
    float scale;
}SpriteRenderItem;

typedef struct{
    float texColStart;
    uint32_t screenCol;
}WallRenderItem;

typedef union{
    SpriteRenderItem spriteItem;
    WallRenderItem wallItem;
}RenderItemData;

typedef struct{
    float zDist;
    //Texture*
    RenderItemType renderItemType;
    RenderItemData renderItemData;
}RenderItemxxxx;


#endif