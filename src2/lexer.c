#include "oto.h"

static bool is_number(int8_t c) {
    if ('0' <= c && c <= '9') return true;
    return false;
}

static bool is_varname(int8_t c) {
    if ('0' <= c && c <= '9') return true;
    if ('a' <= c && c <= 'z') return true;
    if ('A' <= c && c <= 'Z') return true;
    if (c == '_') return true;
    if (c == '#') return true;
    return false;
}

static bool is_symbol(int8_t c) {
    if (strchr("=+-*/%<>!", c) != 0) return true;
    return false;
}

static size_t count_constant_len(int8_t *s) {
    size_t len = 0;
    bool is_float = false;

    while (1) {
        if (is_number(s[len])) {
            len++;
        } else if (s[len] == '.' && !is_float) {
            is_float = true;
            len++;
        } else {
            break;
        }
    }

    return len;
}

static size_t count_varname_len(int8_t *s) {
    size_t len = 0;
    while (is_varname(s[len])) {
        len++;
    }

    return len;
}

static size_t count_operator_len(int8_t *s) {
    size_t len = 0;
    while (is_symbol(s[len]) && s[len] != '\0') {
        len++;
    }

    // 有効な演算子かどうかチェック
    // int32_t op_num = GET_ARRAY_LENGTH()
}