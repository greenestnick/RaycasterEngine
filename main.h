#ifndef MAIN
#define MAIN

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#define SCREEN_WIDTH 600
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


typedef struct{
  float x;
  float y;
}fvec2;

typedef struct{
  int x;
  int y;
}ivec2;

#define vec2_add(a, b, c)\
          c.x = a.x + b.x;\
          c.y = a.y + b.y;

#define vec2_scale(a, b, c)\
        c.x = a.x * b;\
        c.y = a.y * b;

#define vec2_dot(a, b, c)\
        c = a.x*b.x + a.y*b.y; 

#define vec2_print(a)\
        printf("(%f, %f)\n", (float)a.x, (float)a.y);


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

#define MAPSIZE 24
int map[MAPSIZE * MAPSIZE]=
{
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1,
  1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

#endif