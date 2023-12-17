#ifndef BTK_READ_H
#define BTK_READ_H

#include "btk_core.h"

typedef struct btk_read_t {
    char* str;
    int size;
    int pos;
} btk_read;

void btk_read_init(btk_ctx* ctx, btk_read* read, char* str, int size);
char* btk_read_line(btk_ctx* ctx, btk_read* read, int* out_line_len);
int btk_read_int(btk_ctx* ctx, btk_read* read);

#endif // BTK_READ_H