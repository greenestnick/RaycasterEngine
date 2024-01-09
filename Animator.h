#ifndef ANIMATOR
#define ANIMATOR

#include <stdint.h>
#include "TextureMap.h"
#include "Definitions.h"

Animator Animator_Init(Texture* tex, uint8_t frameDiv, uint8_t startingTexID, uint8_t numFrames, uint8_t* dirOffsets){
    return (Animator){tex, 0, frameDiv, startingTexID, startingTexID, numFrames, 0, dirOffsets}; 
}

void Animator_GetDirection(Animator* anim, const Sprite*const sprite, const Player*const player, float xDir, float yDir){
    float xDiff = sprite->xPos - player->xPos;
    float yDiff = sprite->yPos - player->yPos;

    int8_t dirDot = xDiff * xDir + yDiff * yDir;
    if(dirDot > 0) dirDot = 1;
    else if(dirDot < 0) dirDot = -1;
    dirDot++;
    dirDot = dirDot << 2;

    int8_t orthoDot = xDiff * -yDir + yDiff * xDir;
    if(orthoDot > 0) orthoDot = 1;
    else if(orthoDot < 0) orthoDot = -1;
    orthoDot++;

    uint8_t dirIndex = dirDot | orthoDot;
    if(dirIndex > 11) return;

    anim->currDirOffset = anim->directionalOffsets[dirIndex];
}

void Animator_NextFrame(Animator*const anim){
    anim->timer++;
    if(anim->timer % anim->frameDiv != 0) return;

    anim->currFrame++;
    if(anim->currFrame - anim->startingFrame >= anim->numFrames) anim->currFrame = anim->startingFrame;

    anim->tex->texID = anim->currFrame + anim->currDirOffset;
}

#endif