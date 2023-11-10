#include "btk_read.h"
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

bool btk_read_line(btk_read* read, const char** out_line, int* out_line_len) {
    while (read->pos < read->size && btk_read_is_whitespace(read->str[read->pos])) {
        read->pos += 1;
    }

    if (read->pos >= read->size) {
        return false;
    }

    if (out_line) {
        *out_line = read->str + read->pos;
    }

    const int start_pos = read->pos;
    int end_pos = read->pos;
    read->pos += 1;

    while (read->pos < read->size && !btk_read_is_newline(read->str[read->pos])) {
        if (!btk_read_is_whitespace(read->str[read->pos])) {
            end_pos = read->pos;
        }
        read->pos += 1;
    }

    if (out_line_len) {
        *out_line_len = end_pos - start_pos + 1;
    }

    return true;
}

bool btk_read_int(btk_read* read, int* out_val) {
    const char* line = NULL;
    int line_len = 0;
    if (!btk_read_line(read, &line, &line_len)) {
        return false;
    }

    int result = 0;
    int factor = 1;
    for (int i = line_len - 1; i >= 0; --i) {
        if (line[i] < '0' || line[i] > '9') {
            return false;
        }
        result += (int)(line[i] - '0') * factor;
        factor *= 10;
    }

    if (out_val) {
        *out_val = result;
    }

    return true;
}