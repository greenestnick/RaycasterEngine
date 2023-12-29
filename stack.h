#ifndef STACK
#define STACK

#include <stdint.h>
#include "main.h"

#define MAX_STACK_SIZE 128

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
    if(stack->isEmpty) return NULL;

    if(stack->stack_top) stack->stack_top--;
    else                 stack->isEmpty = 1;

    return stack->stack_array[stack->stack_top];
}

RayHit* Peek(Stack* stack){
    return (RayHit*)(stack->stack_array + stack->stack_top);
}

#endif