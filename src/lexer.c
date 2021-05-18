#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "token.h"

/**
 *  変数名として使用可能かどうかを判定する
 *  使えるのであれば1を返す
 *  
 *  現状では定数も変数名として扱っている
 */
int isValNameAvailable(unsigned char c)
{
    if ('0' <= c && c <= '9') return 1;
    if ('a' <= c && c <= 'z') return 1;
    if ('A' <= c && c <= 'Z') return 1;
    return 0;
}

const char operator[] = "=+-*/!%&~|<>?:.#";

/**
 * 文字が演算子であるかどうかを判定する
 * もしそうなら1を返す
 */
int isCharOperator(unsigned char c)
{
    if (strchr(operator, c) != 0) {
        return 1;
    }
    return 0;
}

/* プログラムをトークンコード列に変換する */
int lexer(String s, tokenBuf_t *tcBuf)
{
    int i = 0;        // 現在参照している文字の位置
    int tcCnt = 0;    // これまでに変換したトークンの数

    while (1) {
        if (s[i] == 0) {
            return tcCnt;
        }
        
        if (s[i] == ' ' || s[i] == '\n' || s[i] == '\t' || s[i] == '\r') {    // 読み飛ばしていい文字 
            i++;
            continue;

        } else if (s[i] == '/' && s[i + 1] == '*') {    // コメント部分は飛ばす
            i = i + 2;    // "/*"を飛ばす
            while (!(s[i] == '*' && s[i + 1] == '/')) i++;
            continue; 
        }

        int len = 0;    // 変数などの長さを記録するための変数
        if (strchr("(){}[];,", s[i]) != 0) {
            len = 1;

        } else if (isValNameAvailable(s[i])) {    // 変数か定数
            while (isValNameAvailable(s[i + len])) len++;

        } else if (isCharOperator(s[i]) != 0) {    // 演算子
            while (isCharOperator(s[i + len]) != 0 && s[i + len] != 0) len++;

        } else {
            // TODO: 真面目にエラー原因を書く
            printf("syntax error\n");
            exit(1);
        }

        getTc(&s[i], len, tcBuf);
        i += len;
        tcCnt++;
    }
}