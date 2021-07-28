#include "lexer.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token.h"
#include "../errorHandle.h"
#include "../utils/util.h"
#include "../variable/variable.h"

/* 変数名として使用可能かどうかを判定する */
int32_t isValNameAvailable(unsigned char c) {
    // TODO?: ♯や♭を変数名に使えるようにする
    if ('0' <= c && c <= '9') return 1;
    if ('a' <= c && c <= 'z') return 1;
    if ('A' <= c && c <= 'Z') return 1;
    if (c == '_') return 1;
    return 0;
}

/* 定数かどうかを判定する */
int32_t isConst(unsigned char c) {
    if ('0' <= c && c <= '9') return 1;
    return 0;
}

const str_t symbol = "=+-*/!%&~|<>?:.#";
/* 文字が演算子であるかどうかを判定する */
int32_t isCharOperator(unsigned char c) {
    if (strchr(symbol, c) != 0) return 1;
    return 0;
}

static const str_t operators[] = {
    "<-", "->", "==", "!=", "<", ">=", "<=", ">",
    "+", "-", "*", "/", "%", "=", "&&", "||"
};

/* s[i] ~ s[i + len]の文字列が演算子なのかを判定する */
int32_t isTrueOperator(str_t s, int32_t i, int32_t len) {
    int opNum = GET_ARRAY_LENGTH(operators);
    for (int i = 0; i < opNum; i++) {
        if (strncmp(operators[i], s, len) == 0) {
            return 1;
        }
    }
    return 0;
}

/* プログラムをトークンコード列に変換する */
int32_t lexer(str_t s, tokenBuf_t *tcBuf, var_t *var) {
    int32_t i     = 0;  // 現在参照している文字の位置
    int32_t tcCnt = 0;  // これまでに変換したトークンの数

    while (1) {
        if (s[i] == 0 || s[i] == '\0') {
            tcBuf->tc[tcCnt] = TcLF;
            return tcCnt;
        }

        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\r') {  // 読み飛ばしていい文字
            i++;
            continue;

        } else if (s[i] == '#') {
            while (!(s[i] == '\n')) i++;
            continue;
        }

        int32_t len = 0;  // 変数などの長さを記録するための変数
        int32_t type = TyVoid;  // そのトークンが何の種類なのかを記録するための変数
        if (strchr("(){}[]:,\n", s[i]) != 0) {
            len = 1;

        } else if (isConst(s[i])) {  // 定数
            type = TyConst;
            int32_t decimal = 0;  // 小数点の出現回数
            while (1) {
                if (isConst(s[i + len])) {
                    len++;
                } else if (s[i + len] == '.' && decimal == 0) {
                    decimal = 1;
                    len++;
                } else {
                    break;
                }
            }

        } else if (isValNameAvailable(s[i])) {  // 変数
            while (isValNameAvailable(s[i + len])) len++;

        } else if (isCharOperator(s[i]) != 0) {  // 演算子
            while (isCharOperator(s[i + len]) != 0 && s[i + len] != 0) {
                len++;
            }

            if (!isTrueOperator(&s[i], i, len)) {
                callError(SYNTAX_ERROR);
            }

        } else {
            callError(SYNTAX_ERROR);
        }

        tcBuf->tc[tcCnt] = getTc(&s[i], len, tcBuf, var, type);
        i += len;
        tcCnt++;
    }
}