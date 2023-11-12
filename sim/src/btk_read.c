#include "btk_read.h"
#include "btk_ctx.h"
#include <stddef.h>

void btk_read_init(btk_read* read, const char* str, int size) {
    read->str = str;
    read->size = size;
    read->pos = 0;
}

static bool btk_read_is_newline(char c) {
    return c == '\r' || c == '\n';
}

static bool btk_read_is_whitespace(char c) {
    return btk_read_is_newline(c) || c == ' ' || c == '\t';
}

void btk_read_line(btk_read* read, const char** out_line, int* out_line_len) {
    const btk_host* host = btk_ctx_host();

    while (read->pos < read->size && btk_read_is_whitespace(read->str[read->pos])) {
        read->pos++;
    }

    if (read->pos >= read->size) {
        host->panic(host->ctx, "read_line reached EOF!");
    }

    if (out_line) {
        *out_line = read->str + read->pos;
    }

    const int start_pos = read->pos;
    int end_pos = read->pos;
    read->pos++;

    while (read->pos < read->size && !btk_read_is_newline(read->str[read->pos])) {
        if (!btk_read_is_whitespace(read->str[read->pos])) {
            end_pos = read->pos;
        }
        read->pos++;
    }

    if (out_line_len) {
        *out_line_len = end_pos - start_pos + 1;
    }
}

void btk_read_int(btk_read* read, int* out_val) {
    const btk_host* host = btk_ctx_host();

    const char* line = NULL;
    int line_len = 0;
    btk_read_line(read, &line, &line_len);

    bool is_neg = false;
    if (line[0] == '-' && line_len > 1) {
        is_neg = true;
        line++;
        line_len--;
    }

    int res = 0;
    for (int i = 0; i < line_len; ++i) {
        if (line[i] < '0' || line[i] > '9') {
            host->panic(host->ctx, "read_int invalid character!");
        }

        int prv_res = res;
        res = (res * 10) + (int)(line[i] - '0');

        // check for overflow
        if (res < prv_res) {
            host->panic(host->ctx, "read_int overflow!");
        }
    }

    if (out_val) {
        *out_val = is_neg ? -res : res;
    }
}