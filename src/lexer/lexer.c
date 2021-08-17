#include "lexer.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "tokencode.h"
#include "../error/error.h"
#include "../utils/util.h"
#include "../variable/variable.h"

#define IS_IGNORE_CHAR(c) c == ' ' || c == '\t' || c == '\r'

bool_t is_number(unsigned char c) {
    if ('0' <= c && c <= '9') return 1;
    return 0;
}

uint32_t get_const_len(str_t s) {
    uint32_t len = 0;
    bool_t is_float = false;

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

bool_t is_varname_available(unsigned char c) {
    if ('0' <= c && c <= '9') return 1;
    if ('a' <= c && c <= 'z') return 1;
    if ('A' <= c && c <= 'Z') return 1;
    if (c == '_') return 1;
    if (c == '#') return 1;
    return 0;
}

uint32_t get_varname_len(str_t s) {
    int32_t len = 0;
    while (is_varname_available(s[len])) {
        len++;
    }
    return len;
}

static const str_t symbol = "=+-*/!%&~|<>";
bool_t is_symbol(unsigned char c) {
    if (strchr(symbol, c) != 0) return 1;
    return 0;
}

static const str_t operators[] = {
    "<-", "->", "==", "!=", "<", ">=", "<=", ">",
    "+", "-", "*", "/", "%", "="
};

/* s[i] ~ s[i + len]の文字列が演算子なのかを判定する */
bool_t is_true_operator(str_t s, int32_t i, int32_t len) {
    int op_num = GET_ARRAY_LENGTH(operators);
    for (int i = 0; i < op_num; i++) {
        if (strncmp(operators[i], s, len) == 0) return 1;
    }
    return 0;
}

uint32_t get_operator_len(str_t s) {
    uint32_t len = 0;
    while (is_symbol(s[len]) && s[len] != '\0') {
        len++;
    }

    if (!is_true_operator(s, 0, len)) {
        call_error(SYNTAX_ERROR);
    }

    return len;
}

uint32_t lexer(str_t s, uint32_t fsize, tokenbuf_t *tcbuf, var_t *var_list) {
    uint32_t i   = 0;  // 現在参照している文字の位置
    uint32_t cnt = 0;  // これまでに変換したトークンの数

    while (i < fsize) {
        if (IS_IGNORE_CHAR(s[i])) {
            // 読み飛ばしていい文字
            i++;
            continue;

        } else if (s[i] == '/' && s[i + 1] == '*') {
            while (!(s[i] == '*' && s[i + 1] == '/')) {
                i++;
            }
            i = i + 2;
            continue;
        }

        uint32_t len = 0;  // トークン文字列の長さ
        uint32_t type = TyVoid;
        if (strchr("(){}[]:,\n", s[i]) != 0) {
            len = 1;

        } else if (is_number(s[i])) {  // 定数
            type = TyConst;
            len  = get_const_len(&s[i]);

        } else if (is_varname_available(s[i])) {  // 変数
            len = get_varname_len(&s[i]);

        } else if (is_symbol(s[i]) ) {  // 演算子
            len = get_operator_len(&s[i]);

        } else {
            call_error(SYNTAX_ERROR);
        }

        tcbuf->tc_list[cnt] = get_tc(tcbuf, var_list, &s[i], len, type);
        i += len;
        cnt++;
    }

    tcbuf->tc_list[cnt] = TcLF;
    return cnt;
}
