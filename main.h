#ifndef MAIN
#define MAIN

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "stack.h"
#include "map.h"
#include "RenderList.h"

#define SCREEN_WIDTH 711
#define SCREEN_HEIGHT 400

#define WIN_TITLE "Window Title Undefined"

#define MAX_SLOPE 10000000000
#define TEX_SIZE 32

#define UPDATE_TIMER_MS 16 //about 60 ticks per second

//TODO: Define a macro-function to get a color from the textSurf taking in u,v, and texId

typedef struct{
    float xPos;
    float yPos;
    float xDir;
    float yDir;
    float xPlane;
    float yPlane;
    float pitch;
}Player;

typedef struct{
  Uint32 x;
  Uint32 y;
  int xVel;
  int yVel;
}Mouse;

typedef struct{
  Uint8 spriteTextureID;
  float xPos;
  float yPos;
  float heightAdjust;
  float scale;
  float xCamPos;
  float yCamPos;
}Sprite;


Uint32 AlphaBlend(Uint32 top, Uint32 bottom){
  Uint8 alpha = (top >> 24);
  
  switch(alpha){
    case 0: return bottom;
    case 255: return top;
  }

  Uint8 invAlpha = 255 - (Uint8)alpha;
  Uint8 rt, gt, bt, rb, bb, gb;
  rb = (Uint8)(bottom >> 16);
  rt = (Uint8)(top >> 16);
  gb = (Uint8)(bottom >> 8);
  gt = (Uint8)(top >> 8);
  bb = (Uint8)bottom;
  bt = (Uint8)top;

  Uint32 nr = (rt*alpha + rb*invAlpha) >> 8; 
  Uint32 ng = (gt*alpha + gb*invAlpha) >> 8;
  Uint32 nb = (bt*alpha + bb*invAlpha) >> 8;   

  return 0xFF000000 | (nr << 16) | (ng << 8) | (nb);
}

#endif