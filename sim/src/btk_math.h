#ifndef BTK_MATH_H
#define BTK_MATH_H

#include "btk_core.h"

typedef struct btk_vec2_t {
    float x;
    float y;
} btk_vec;

btk_vec btk_vec_add(btk_vec a, btk_vec b);
btk_vec btk_vec_mul(btk_vec a, btk_vec b);

typedef struct btk_rect_t {
    float x;
    float y;
    float w;
    float h;
} btk_rect;

#endif // BTK_MATH_H