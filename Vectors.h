#ifndef VECTORS
#define VECTORS

typedef struct{
    float x;
    float y;
}vec2;

void vec2add(vec2* a, vec2* b, vec2* c){
    c->x = a->x + b->x;
    c->y = a->y + b->y;
}


#endif