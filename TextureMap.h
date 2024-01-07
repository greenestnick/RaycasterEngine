#ifndef TEXTUREMAP
#define TEXTUREMAP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct{
    Uint8 texSize;
    Uint8 rows;
    Uint8 cols;
    Uint32 numTextures;
    SDL_Surface* surface;
}TextureMap;


typedef struct{
    Uint8 texID;
    TextureMap* texMap;
}Texture;

#define Texture_Make(texId, texMapPtr) (Texture){texId, texMapPtr}

TextureMap Texture_Init(char* path, Uint8 texSize, Uint8 rows, Uint8 cols, Uint32 numTextures){
    SDL_Surface* surf = IMG_Load(path);
    if(surf->format->format != SDL_PIXELFORMAT_ARGB8888){
        surf = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_ARGB8888, 0);
    }

    return (TextureMap){texSize, rows, cols, numTextures, surf};
}

void Texture_Destroy(TextureMap* texMap){
    if(texMap->surface != NULL)
        SDL_FreeSurface(texMap->surface);
}

Uint32 Texture_DirectSample(const TextureMap*const  texMap, Uint32 x, Uint32 y, Uint32 u, Uint32 v){
    Uint32* sample = (Uint32*)texMap->surface->pixels + (x * texMap->texSize) + (texMap->texSize *  texMap->surface->w * y) + u + (texMap->surface->w * v);
    return *sample;
}

Uint32 Texture_Sample(const TextureMap*const texMap, Uint32 texNum, Uint32 u, Uint32 v){
    Uint32 x = texNum % texMap->cols;
    Uint32 y = ((float)texNum / texMap->cols);

    Uint32* sample = (Uint32*)texMap->surface->pixels + (x * texMap->texSize) + (texMap->texSize *  texMap->surface->w * y) + u + (texMap->surface->w * v);
    return *sample;
}

/*
Uint32 Texture_SampleRef(const Texture*const tex, Uint32 u, Uint32 v){
    TextureMap* texMap = tex->texMap;

    Uint32 x = tex->texID % texMap->cols;
    Uint32 y = ((float)tex->texID / texMap->cols);

    Uint32* sample = (Uint32*)texMap->surface->pixels + (x * texMap->texSize) + (texMap->texSize *  texMap->surface->w * y) + u + (texMap->surface->w * v);
    return *sample;
}
*/

#endif