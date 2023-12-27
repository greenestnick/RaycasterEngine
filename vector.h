#ifndef VECTOR
#define VECTOR

#include <stdint.h>

typedef struct{
    float x;
    float y;
}fvec2;

typedef struct{
    int x;
    int y;
}ivec2;

typedef struct{
    uint32_t x;
    uint32_t y;
}uivec2;

typedef union{
     fvec2 fvec;
     ivec2 ivec;
     uivec2 uivec;
}vec2;


#define vec2_add_comp(a, x, y, c)\
        c.x = a.x + x;\
        c.y = a.y + y;

#define vec2_add(a, b, c)\
        c.x = a.x + b.x;\
        c.y = a.y + b.y;

#define vec2_scale(a, b, c)\
        c.x = a.x * b;\
        c.y = a.y * b;

#define vec2_dot(a, b, c)\
        c = a.x*b.x + a.y*b.y; 

#define vec2_hadamard(a, x, y, c)\
        c.x = a.x * x;\
        c.y = a.y * y;

#endif