#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "preprocess.h"
#include "../util/util.h"
#include "../error/error.h"
#include "../token/token.h"
#include "../variable/type.h"

static bool is_number(int8_t c) {
    if ('0' <= c && c <= '9') return 1;
    return 0;
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

static bool is_varname(int8_t c) {
    if ('0' <= c && c <= '9') return 1;
    if ('a' <= c && c <= 'z') return 1;
    if ('A' <= c && c <= 'Z') return 1;
    if (c == '_') return 1;
    if (c == '#') return 1;
    return 0;
}

static size_t count_varname_len(int8_t *s) {
    size_t len = 0;
    while (is_varname(s[len])) len++;
    return len;
}

static const int8_t *symbol = "=+-*/%<>!";
static bool is_symbol(int8_t c) {
    if (strchr(symbol, c) != 0) return 1;
    return 0;
}

static const int8_t *operators[] = {
    "<-", "->", "=", "+", "-", "*", "/", "%", "==", "!=", "<", ">=", "<=", ">"
};
static size_t count_operator_len(int8_t *s) {
    size_t len = 0;
    while (is_symbol(s[len]) && s[len] != '\0') len++;

    // 有効な演算子かどうかチェックする
    int32_t op_num = GET_ARRAY_LENGTH(operators);
    for (int32_t i = 0; i < op_num; i++) {
        if (strncmp(operators[i], s, len) == 0) return len;
    }

    return -1;
}

#define CAN_IGNORE_CHAR(c) c == ' ' || c == '\t' || c == '\r'
#define IS_PREPROCESS(c) c == '@'

uint32_t lexer(int8_t *s) {
    init_token();

    uint32_t i   = 0;  // 現在参照している文字の位置
    uint32_t cnt = get_conv_source_size();  // これまでに変換したトークンの数
    
    while (s[i] != 0) {
        if (CAN_IGNORE_CHAR(s[i])) {
            i++;
            continue;
        } else if (s[i] == '/' && s[i + 1] == '*') {  // コメント
            while (!(s[i] == '*' && s[i + 1] == '/')) i++;
            // '/'の分も含めてi += 2
            i += 2;
            continue;
        }

        size_t len  = 0;
        type_t type = TyVoid;
        if (IS_PREPROCESS(s[i])) {
            i += preprocess(&s[i]);
            // もしincludeした時には変更されているのでcntを更新する
            cnt = get_conv_source_size();
            continue;

        } else if (strchr("(){}[]:,\n", s[i]) != 0) {
            len = 1;

        } else if (is_number(s[i])) {  // 定数
            type = TyConst;
            len  = count_constant_len(&s[i]);

        } else if (is_varname(s[i])) {  // 変数
            len = count_varname_len(&s[i]);

        } else if (is_symbol(s[i])) {  // 演算子
            len = count_operator_len(&s[i]);
            if (len == -1) {
                call_error(SYNTAX_ERROR);
            }

        } else {
            call_error(SYNTAX_ERROR);
        }

        set_tc(allocate_tc(&s[i], len, type), cnt);
        i += len;
        cnt++;
    }

    // 末尾に改行を入れておく
    set_tc(TcLF, cnt++);

    return cnt;
}