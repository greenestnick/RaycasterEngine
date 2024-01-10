#ifndef VECTOR
#define VECTOR

typedef struct{
    float x;
    float y;
}vec2;

typedef struct{
    float x1;
    float y1;
    float x2;
    float y2;
}mat4;

#define MakeVec(x, y)((vec2){x, y})
#define MakeMat(x1, y1, x2, y2)((mat4){x1, y1, x2, y2})
#define PerpVec(a)((vec2){-a.y, a.x})

vec2 VecAdd(vec2 a, vec2 b){
    return (vec2){a.x + b.x, a.y + b.y};
}

vec2 VecAddDirect(vec2 a, float x, float y){
    return (vec2){a.x + x, a.y + y};
}

vec2 VecSub(vec2 a, vec2 b){
    return (vec2){a.x - b.x, a.y - b.y};
}

vec2 VecScale(vec2 a, float c){
    return (vec2){a.x*c, a.y*c};
}

float VecDot(vec2 a, vec2 b){
    return (a.x*b.x + a.y*b.y);
}

vec2 VecCross(vec2 a, vec2 b){
    return (vec2){a.x*b.y - a.y*b.x};
}

vec2 VecMatrixMult(vec2 a, mat4 M){
    return (vec2){a.x*M.x1 + a.y*M.y1, a.x*M.x2 + a.y*M.y2};
}

#endif