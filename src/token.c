#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"

/* トークン保存領域を新しく作る(いらないかも) */
tokenBuf_t *newTokenBuf()
{
    tokenBuf_t *buf = (tokenBuf_t *)malloc(sizeof(tokenBuf_t));
    if (buf == NULL) {
        fprint32_tf(stderr, "malloc error\n");
        exit(1);
    }
    return buf;
}


/* トークンのメモリ領域を解放する(トークンコード列もついでに解放してくれる) */
void freeTokenBuf(tokenBuf_t *tcBuf)
{
    for (int32_t i = tcBuf->tcs; i > 0; i--) {
        free(tcBuf->tokens[i]);
    }
}


/* トークンを新しく作る */
struct token *newToken(int32_t tc, int32_t len, str_t s)
{
    struct token *t = (struct token *)malloc(sizeof(struct token));
    if (t == NULL) {
        fprint32_tf(stderr, "malloc error\n");
        exit(1);
    }
    t->tc = tc;
    t->tl = len;
    t->ts = s;
    return t;
}


/**
 * トークンの内容(文字列)を記憶するための領域
 * 
 * 文字列の実体はこのバッファに保存されている
 * tcsBuf自体は基本的にtoken.c内部からしか参照しないため気にしなくてもいい
 */
unsigned char tcsBuf[(MAX_TC + 1) * 10];


/**
 * 新しいトークンを作り, tcBuf->tokensに追加する関数
 * 
 * 名前の割にはやってることが多い...要改善
 */
int32_t putTc(int32_t tc, int32_t len, str_t s, tokenBuf_t *tcBuf)
{
    if (tcBuf->tcs >= MAX_TC) {
        fprintf(stderr, "too many tokens\n");
        exit(1);
    }

    strncpy(&tcsBuf[tcBuf->tcb], s, len);    // tcsBufにsをコピー
    tcsBuf[tcBuf->tcb + len] = 0;    // tcsBufの終わりに終端コードを付ける
    tcBuf->tokens[tc] = newToken(tc, len, s);
    tcBuf->tcb += len + 1;
    tcBuf->tcs++;
    return 0;
}


/**
 * トークンコードを得るための関数
 * もし登録されていないなら, 新しく作る
 */
int32_t getTc(str_t s, int32_t len, tokenBuf_t *tcBuf, var_t **var, int32_t type)
{
    int32_t i;
    for (i = 0; i < tcBuf->tcs; i++) {    // 登録済みの中から探す
        if (len == tcBuf->tokens[i]->tl && strncmp(s, tcBuf->tokens[i]->ts, len) == 0) {
            break;
        }
    }

    if (i == tcBuf->tcs) {    // 新規作成時の処理
        putTc(i, len, s, tcBuf);
        // printf("tc : %d, ts : %s, tl : %d\n", i, tcBuf->tokens[i]->ts, len);    // デバッグ用
        
        // var[i] = strtol(tcBuf->tokens[i]->ts, 0, 0);    
        
        // 定数だった場合に初期値を設定
        switch (type) {
        case TyInt:
            varDef(var[i], type, strtol(tcBuf->tokens[i]->ts, 0, 0));
            break;
        case TyFloat:
            varDef(var[i], type, strtod(tcBuf->tokens[i]->ts, 0));
            break;
        case TyConstI:
            varDef(var[i], type, strtol(tcBuf->tokens[i]->ts, 0, 0));
            break;
        case TyConstF:
            varDef(var[i], type, strtod(tcBuf->tokens[i]->ts, 0));
            break;
        }
    } 

    return i;
}


/* 演算子記号などを最初にlexerしておく関数 */
int32_t tcInit(tokenBuf_t *tcBuf, var_t **var)
{
    /* 最初にlexerしておく文字列 */
    str_t symbols = "; . ( ) [ ] { } == != < >= <= > + - * / // % = ++ -- -> , int32_t float print32_t !**! \0";

    return lexer(symbols, tcBuf, var);
}