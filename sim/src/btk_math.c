#include "btk_math.h"

btk_vec btk_vec_add(btk_vec a, btk_vec b) {
    return (btk_vec){ .x = a.x + b.x, .y = a.y + b.y };
}

btk_vec btk_vec_mul(btk_vec a, btk_vec b) {
    return (btk_vec){ .x = a.x * b.x, .y = a.y * b.y };
}