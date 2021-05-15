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


unsigned char *ts[TC_MAX + 1];             // トークンの内容(文字列)を記憶
int tl[TC_MAX + 1];                        // トークンの長さ
unsigned char tcBuf[(TC_MAX + 1) * 10];    // トークン1つあたり平均10バイトを想定
int tcs = 0;                               // 今まで発行したトークンコードの個数
int tcb = 0;                               // tcBuf[]の未使用領域の個数

/* トークンコードを得るための関数 */
int getTc(String s, int len, int var[])
{
    int i;
    for (i = 0; i < tcs; i++) {    // 登録済みの中から探す. あったらそれを返す
        if (len == tl[i] && strncmp(s, ts[i], len) == 0) {
            break;
        }
    }

    if (i == tcs) {    // 登録されていなかった場合は新規登録
        if (tcs >= TC_MAX) {
            printf("too many tokens\n");
            exit(1);
        }
        strncpy(&tcBuf[tcb], s, len);    // トークンの文字列を記憶
        tcBuf[tcb + len] = 0;    // 終端文字コード
        ts[i] = &tcBuf[tcb];
        tl[i] = len;
        tcb += len + 1;
        tcs++;
        var[i] = strtol(ts[i], 0, 0);    // 定数だった場合に初期値を設定
    }

    return i;
}

/* プログラムをトークンコード列に変換する */
int lexer(String s, int tc[], int var[])
{
    int i = 0;    // 今s[]のどこを読んでいるか
    int j = 0;    // これまでに変換したトークン列の長さ(トークン列の末尾のidx)

    while (1) {
        if (s[i] = ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {
            // これらの文字は読み飛ばす
            i++;
            continue;
        }

        if (s[i] == 0) {    // ファイル終端
            return j;
        }

        int len = 0;    // トークンの文字列の長さを記憶するための変数
        if (strchr("(){}[];,", s[i]) != 0) {
            len = 1;

        } else if (isValNameAvailable(s[i])) {
            while (isValNameAvailable(s[i + len])) len++;

        } else if (strchr("=+-*/!%&~|<>?:.#", s[i]) != 0) {
            // TODO: "a =-2"などの文が"a =- 2"と分割されてしまうのでいつか修正
            while (strchr("=+-*/!%&~|<>?:.#", s[i + len]) != 0 && s[i + len] != 0) len++;

        } else {
            printf("syntax error : %.10s\n", &s[i]);
            exit(1);
        }

        tc[j] = getTc(&s[i], len, var);
        i += len;
        j++;
    }
}