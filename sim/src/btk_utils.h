#ifndef BTK_UTILS_H
#define BTK_UTILS_H

#define BTK_SWAP(T, a, b) do { T swap_tmp = a; a = b; b = swap_tmp; } while (0)
#define BTK_MIN(a, b) (a > b) ? b : a
#define BTK_MAX(a, b) (a > b) ? a : b

#endif // BTK_UTILS_H