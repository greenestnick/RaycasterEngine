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
    Uint8 steppingInX;
}RayHit;

typedef struct{
    unsigned short stack_top;
    unsigned short isEmpty;
    RayHit stack_array[MAX_STACK_SIZE];   
}Stack;


Stack StackInit(){
    Stack stack = {0, 1};
    return stack;
}

void Push(Stack* stack, RayHit data){
    stack->isEmpty = 0;
    stack->stack_array[stack->stack_top] = data;
    stack->stack_top++;
}

RayHit Pop(Stack* stack){
    if(stack->isEmpty) (RayHit){.xTile = 1, .yTile = -1};

    if(stack->stack_top) stack->stack_top--;
    else                 stack->isEmpty = 1;

    return stack->stack_array[stack->stack_top];
}

RayHit* Peek(Stack* stack){
    return (RayHit*)(stack->stack_array + stack->stack_top);
}

#endif