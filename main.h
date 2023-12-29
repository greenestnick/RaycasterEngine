#ifndef MAIN
#define MAIN

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "stack.h"
#include "map.h"

#define SCREEN_WIDTH 711
#define SCREEN_HEIGHT 400

#define WIN_TITLE "Window Title Undefined"

#define MAX_SLOPE 10000000000
#define TEX_SIZE 32

#define UPDATE_TIMER_MS 16 //about 60 ticks per second

typedef struct{
    float xPos;
    float yPos;
    float xDir;
    float yDir;
    float xPlane;
    float yPlane;
}Player;

typedef struct{
  Uint32 x;
  int xVel;
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
  Uint8 alphaInt = (top >> 24);
  
  switch(alphaInt){
    case 0: return 0;
    case 255: return top;
  }

  float alpha = alphaInt / 255.0;

  float invAlpha = 1 - alpha;
  
  Uint8 rt, gt, bt, rb,bb,gb;
  rb = (Uint8)(bottom >> 16);
  rt = (Uint8)(top >> 16);
  gb = (Uint8)(bottom >> 8);
  gt = (Uint8)(top >> 8);
  bb = (Uint8)bottom;
  bt = (Uint8)top;   

  return 0xFF000000 | (Uint8)(alpha * rt + invAlpha * rb)<<16 | (Uint8)(alpha * gt + invAlpha * gb)<<8 | (Uint8)(alpha * bt + invAlpha * bb);
}

#endif