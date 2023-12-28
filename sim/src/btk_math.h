#ifndef BTK_MATH_H
#define BTK_MATH_H

#include "btk_core.h"

int btk_min(int a, int b);
float btk_minf(float a, float b);
int btk_max(int a, int b);
float btk_maxf(float a, float b);
int btk_clamp(int x, int min, int max);
float btk_clampf(float x, float min, float max);
int btk_signf(float x);

typedef struct btk_vec_t {
    float x;
    float y;
} btk_vec;

btk_vec btk_vec_add(btk_vec a, btk_vec b);
btk_vec btk_vec_mul(btk_vec a, btk_vec b);

#define BTK_VEC_ZERO (btk_vec){ .x = 0.0f, .y = 0.0f }

typedef struct btk_rect_t {
    float x;
    float y;
    float w;
    float h;
} btk_rect;

#endif // BTK_MATH_H