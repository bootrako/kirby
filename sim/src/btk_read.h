#ifndef BTK_READ_H
#define BTK_READ_H

#include <stdbool.h>

typedef struct btk_read_t {
    const char* str;
    int size;
    int pos;
} btk_read;

void btk_read_init(btk_read* read, const char* str, int size);
bool btk_read_line(btk_read* read, const char** out_line, int* out_line_len);
bool btk_read_int(btk_read* read, int* out_val);

#endif // BTK_READ_H