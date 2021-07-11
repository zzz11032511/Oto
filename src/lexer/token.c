#include "token.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "../utils/util.h"
#include "../variable/variable.h"

/* トークン保存領域を新しく作る(いらないかも) */
tokenBuf_t *newTokenBuf() {
    tokenBuf_t *buf = (tokenBuf_t *)malloc(sizeof(tokenBuf_t));
    if (buf == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    return buf;
}

/* トークンのメモリ領域を解放する(トークンコード列もついでに解放してくれる) */
void freeTokenBuf(tokenBuf_t *tcBuf) {
    for (int32_t i = tcBuf->tcs; i > 0; i--) {
        free(tcBuf->tokens[i]);
    }
}

/* トークンを新しく作る */
struct token *newToken(int32_t tc, int32_t len, str_t s) {
    struct token *t = (struct token *)malloc(sizeof(struct token));
    if (t == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    t->tc = tc;
    t->tl = len;
    t->ts = s;
    return t;
}

/**
 * トークンの内容(文字列)を記憶するための領域
 * 文字列の実体はこのバッファに保存されている
 */
char tcsBuf[(MAX_TC + 1) * 10];

/* 新しいトークンを作り, tcBuf->tokensに追加する関数 */
int32_t putTc(int32_t tc, int32_t len, str_t s, tokenBuf_t *tcBuf) {
    if (tcBuf->tcs >= MAX_TC) {
        fprintf(stderr, "too many tokens\n");
        exit(1);
    }

    strncpy(&tcsBuf[tcBuf->tcb], (char *)s, len);  // tcsBufにsをコピー
    tcsBuf[tcBuf->tcb + len] = 0;  // tcsBufの終わりに終端コードを付ける
    tcBuf->tokens[tc]        = newToken(tc, len, s);
    tcBuf->tcb += len + 1;
    tcBuf->tcs++;
    return 0;
}

/* トークンコードを得るための関数 */
int32_t getTc(str_t s, int32_t len, tokenBuf_t *tcBuf, var_t *var,
              int32_t type) {
    int32_t i;
    for (i = 0; i < tcBuf->tcs; i++) {  // 登録済みの中から探す
        if (len == tcBuf->tokens[i]->tl &&
            strncmp((char *)s, (char *)(tcBuf->tokens[i]->ts), len) == 0) {
            break;
        }
    }

    if (i == tcBuf->tcs) {  // 新規作成時の処理
        putTc(i, len, s, tcBuf);

#ifdef DEBUG
        printf("tc : %d, ts : %s, tl : %d\n", i, tcBuf->tokens[i]->ts, len);
#endif

        // 定数だった場合に型を設定し、初期値を設定する
        var[i].type = type;
        switch (type) {
            case TyConstI:
                var[i].value.iVal = strtol((char *)(tcBuf->tokens[i]->ts), 0, 0);
                break;
            case TyConstF:
                var[i].value.fVal = strtod((char *)(tcBuf->tokens[i]->ts), 0);
                break;
            default:
                var[i].value.iVal = 0;
                break;
        }
    }

    return i;
}

/* 最初にlexerしておく文字列 */
static const str_t symbols = "; . ( ) [ ] { } == != < >= <= > + - * / % = && || ++ -- , int float if else while print !**! \0";

/* 演算子記号などを最初にlexerしておく関数 */
int32_t tcInit(tokenBuf_t *tcBuf, var_t *var) {
    return lexer(symbols, tcBuf, var);
}