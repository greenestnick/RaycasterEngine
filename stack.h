#ifndef STACK
#define STACK

#include <stdint.h>

#define MAX_STACK_SIZE 128

typedef struct{
    float xRay;
    float yRay;
    float xRayLength;
    float yRayLength;
    Uint32 xTile;
    Uint32 yTile;
    Uint32 column;
    Uint8 steppingInX;
}RayHit;

typedef struct{
    unsigned short head;
    unsigned short isEmpty;
    RayHit stack_array[MAX_STACK_SIZE];   
}Stack;


Stack StackInit(){
    Stack stack = {0, 1};
    return stack;
}

void Push(Stack*const stack, RayHit data){
    stack->isEmpty = 0;
    stack->stack_array[stack->head] = data;
    stack->head++;
}

RayHit Pop(Stack*const stack){
    if(stack->isEmpty) (RayHit){.xTile = 1, .yTile = -1};

    if(stack->head) stack->head--;
    else            stack->isEmpty = 1;

    return stack->stack_array[stack->head];
}

RayHit* Peek(const Stack*const stack){
    return (RayHit*)(stack->stack_array + stack->head);
}

#endif