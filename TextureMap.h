#ifndef TEXTUREMAP
#define TEXTUREMAP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
    Uint8 texSize;
    Uint8 rows;
    Uint8 cols;
    SDL_Surface* surface;
}TextureMap;


TextureMap TextureInit(char* path, Uint8 texSize, Uint8 rows, Uint8 cols){
    SDL_Surface* surf = IMG_Load(path);
    if(surf->format->format != SDL_PIXELFORMAT_ABGR8888){
        surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ABGR8888, 0);
    }

    return (TextureMap){texSize, rows, cols, surf};
}

void TextureDestroy(TextureMap* texMap){
    if(texMap->surface != NULL)
        SDL_FreeSurface(texMap->surface);
}

Uint32 TextureMapSample(TextureMap* texMap, Uint8 x, Uint8 y, Uint8 u, Uint8 v){
    Uint32* sample = texMap->surface->pixels + (x * texMap->texSize) + (texMap->surface->w * texMap->texSize * y) + u + (texMap->texSize * v);
    return *sample;
}

#endif