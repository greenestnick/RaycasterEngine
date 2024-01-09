#ifndef DEFINITIONS
#define DEFINITIONS

#include "TextureMap.h"

typedef struct{
    Texture* tex;
    uint32_t timer;
    uint8_t frameDiv;
    uint8_t currFrame;
    uint8_t startingFrame;
    uint8_t numFrames;
    uint8_t currDirOffset;
    uint8_t* directionalOffsets;
}Animator;

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
  Texture tex;
  float xPos;
  float yPos;
  float heightAdjust;
  float scale;
  float xCamPos;
  float yCamPos;
}Sprite;

#endif