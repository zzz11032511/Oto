#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

/* 変数名に使用出来る名前かどうか判定する */
int isValNameAvailable(unsigned char c)
{
    // TODO: 予約語も判定するようにする
    if ('0' <= c && c <= '9') return 1;
    if ('a' <= c && c <= 'z') return 1;
    if ('A' <= c && c <= 'Z') return 1;
    if (c == '_') return 1;
    return 0;
}

unsigned char *ts[MAX_TC + 1];             // トークンの内容(文字列)を記憶
int tl[MAX_TC + 1];                        // トークンの長さ
unsigned char tcBuf[(MAX_TC + 1) * 10];    // トークン1つあたり平均10バイトを想定
int tcs = 0;                               // 今まで発行したトークンコードの個数
int tcb = 0;                               // tcBuf[]の未使用領域の個数

int var[MAX_TC + 1];    // 変数

/**
 * トークンコードを得るための関数
 * 
 * params:
 *      s   : 対象の文字列
 *      len : 文字列の長さ
 * 
 * return:
 *      トークンコード
 */
int getTc(unsigned char *s, int len)
{
    int i;
    for (i = 0; i < tcs; i++) {    // 登録済みの中から探す
        if (len == tl[i] && strncmp(s, ts[i], len) == 0) {
            break;
        }
    }

    if (i == tcs) {    // 登録されていなかった場合は新規登録
        if (tcs >= MAX_TC) {
            printf("too many tokens\n");
            exit(1);
        }
        strncpy(&tcBuf[tcb], s, len);
        tcBuf[tcb + len] = 0;    // 終端文字コード
        
        ts[i] = &tcBuf[tcb];
        tl[i] = len;
        tcb += len + 1;
        tcs++;

        var[i] = strtol(ts[i], 0, 0);    // 定数だった場合に初期値を設定
    }

    return i;
}

/**
 * プログラムをトークンコード列に変換する
 * 
 * params:
 *      s  : プログラム本体(txt)
 *      tc : トークンコード格納用の配列
 * 
 * return:
 *      これまでに変換したトークン列の長さ
 */
int lexer(unsigned char *s, int tc[])
{
    int len;
    int i = 0;    // 今s[]のどこを呼んでいるか
    int j = 0;    // これまでに変換したトークン列の長さ

    const char *operators = "=+-*/!%&~|<>?:.#";    // 演算子群

    while (1) {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {
            i++;
            continue;
        } 

        if (s[i] == 0)    // ファイル終端
            return j;

        len = 0;
        if (strchr("(){}[];,", s[i]) != 0) {
            len = 1;    // これらの記号は1文字でトークンとみなす

        } else if (isValNameAvailable(s[i])) {    // 変数名か定数
            while (isValNameAvailable(s[i + len])) len++;
        
        } else if (strchr(operators, s[i]) != 0) {    // 演算子
            while (strchr(operators, s[i + len]) != 0 && s[i + len] != 0) len++;
        
        } else {
            printf("syntax error : %.10s\n", &s[i]);
            exit(1);
        }

        // TODO: 予約語のlexer処理もちゃんと書く
        tc[j] = getTc(&s[i], len);
        i += len;
        j++;
    }
}