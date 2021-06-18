#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"

/* トークン保存領域を新しく作る(いらないかも) */
tokenBuf_t *newTokenBuf()
{
    tokenBuf_t *buf = (tokenBuf_t *)malloc(sizeof(tokenBuf_t));
    if (buf == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    return buf;
}


/* トークンのメモリ領域を解放する(トークンコード列もついでに解放してくれる) */
void freeTokenBuf(tokenBuf_t *tcBuf)
{
    for (int i = tcBuf->tcs; i > 0; i--) {
        free(tcBuf->tokens[i]);
    }
}


/* トークンを新しく作る */
struct token *newToken(int tc, int len, String s)
{
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
int putTc(int tc, int len, String s, tokenBuf_t *tcBuf)
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
int getTc(String s, int len, tokenBuf_t *tcBuf, int *var, int type)
{
    int i;
    for (i = 0; i < tcBuf->tcs; i++) {    // 登録済みの中から探す
        if (len == tcBuf->tokens[i]->tl && strncmp(s, tcBuf->tokens[i]->ts, len) == 0) {
            break;
        }
    }

    if (i == tcBuf->tcs) {    // 新規作成
        putTc(i, len, s, tcBuf);
        // printf("tc : %d, ts : %s, tl : %d\n", i, tcBuf->tokens[i]->ts, len);    // デバッグ用
        var[i] = strtol(tcBuf->tokens[i]->ts, 0, 0);    // 定数だった場合に初期値を設定
    } 

    return i;
}


/* 演算子記号などを最初にlexerしておく関数 */
int tcInit(tokenBuf_t *tcBuf, int *var)
{
    /* 最初にlexerしておく文字列 */
    String symbols = "; . ( ) [ ] { } == != < >= <= > + - * / // % = ++ -- -> , int float print !**! \0";

    return lexer(symbols, tcBuf, var);
}