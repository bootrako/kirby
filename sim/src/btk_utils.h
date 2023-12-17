#ifndef BTK_UTILS_H
#define BTK_UTILS_H

#include <btk_sim.h>

#define BTK_SWAP(T, a, b) T swap_tmp = a; a = b; b = swap_tmp
#define BTK_MIN(a, b) (((a) > (b)) ? b : a)
#define BTK_MAX(a, b) (((a) > (b)) ? a : b)
#define BTK_CLAMP(x, min, max) BTK_MAX(min, BTK_MIN(max, x))
#define BTK_REF(param) (void)(param)

#endif // BTK_UTILS_H