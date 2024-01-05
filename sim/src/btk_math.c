#include "btk_math.h"

int btk_min(int a, int b) {
    return a < b ? a : b;
}

float btk_minf(float a, float b) {
    return a < b ? a : b;
}

int btk_max(int a, int b) {
    return a < b ? b : a;
}

float btk_maxf(float a, float b) {
    return a < b ? b : a;
}

int btk_clamp(int x, int min, int max) {
    return btk_max(btk_min(x, max), min);
}

float btk_clampf(float x, float min, float max) {
    return btk_maxf(btk_minf(x, max), min);
}

int btk_signf(float x) {
    return x > 0.0f ? 1 : -1;
}

btk_vec btk_vec_add(btk_vec a, btk_vec b) {
    return (btk_vec){ .x = a.x + b.x, .y = a.y + b.y };
}

btk_vec btk_vec_sub(btk_vec a, btk_vec b) {
    return (btk_vec){ .x = a.x - b.x, .y = a.y - b.y };
}

btk_vec btk_vec_mul(btk_vec a, btk_vec b) {
    return (btk_vec){ .x = a.x * b.x, .y = a.y * b.y };
}