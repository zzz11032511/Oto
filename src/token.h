#ifndef INCLUDED_TOKEN
#define INCLUDED_TOKEN 1

#include "util.h"

#define MAX_TC 255

#define TC_LIST_SIZE 10000


// unsigned char tcsBuf[]  トークンの内容(文字列)を記憶するための領域. token.cにある

struct token {
    int tc;    // トークン番号
    int tl;    // トークンの文字列の長さ
    String ts;    // トークンの内容(文字列)を記憶, tcsBuf(token.c内)へのポインタ
};

typedef struct tokenBuf {
    int tc[TC_LIST_SIZE];    // 文字列をトークンコード列に変換したものを入れる
    struct token *tokens[MAX_TC];    // トークンコード列
    int tcs;    // 今まで発行したトークンコードの個数
    int tcb;    // tcsBuf[]の未使用領域
} tokenBuf_t;

/* トークン保存領域を新しく作る */
tokenBuf_t *newTokens();

/* トークンのメモリ領域を解放する(トークンコード列もついでに解放してくれる) */
void freeTokens(tokenBuf_t *tcBuf);

/**
 * トークンコードを得るための関数
 * もし登録されていないなら, 新しく作る
 */
int getTc(String s, int len, tokenBuf_t *tcBuf);

/* 演算子記号などを最初にlexerしておく関数 */
int tcInit(tokenBuf_t *tcBuf);

#endif