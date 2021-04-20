#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char *String;

// コマンドライン引数で指定されたソースファイルをtに読み込む
void loadText(int argc, const char **argv, String t, int siz)
{
    FILE *fp;
    int i;
    
    if (argc < 2) {    // 引数が少ないのでエラー表示
        printf("usage>%s program-file\n", argv[0]);
        exit(1);
    }

    fp = fopen(argv[1], "rt");    // テキストモードでファイルを開く
    if (fp == 0) {    // ファイルを開けなかった
        printf("fopen error :  %s\n", argv[1]);
        exit(1);
    }
    i = fread(t, 1, siz - 1, fp);    // iは読み込んだバイト数

    fclose(fp);
    t[i] = 0;    // 終端マーク
}


#define MAX_TC 1000       // トークンコードの最大値
String ts[MAX_TC + 1];    // トークンの内容(文字列)を記憶
int tl[MAX_TC + 1];       // トークンの長さ
unsigned char tcBuf[(MAX_TC + 1) * 10];    // トークン1つあたり平均10バイトを想定
int tcs = 0;    // 今まで発行したトークンコードの個数
int tcb = 0;    // tcBuf[]の未使用領域

int var[MAX_TC + 1];    // 変数

// トークン番号を得るための関数
int getTc(String s, int len)
{
    int i;
    for (i = 0; i < tcs; i++) {    // 登録済みの中から探す
        if (len == tl[i] && strncmp(s, ts[i], len) == 0) {
            break;
        }
    }

    if (i == tcs) {
        if (tcs >= MAX_TC) {
            printf("too many tokens\n");
            exit(1);
        }
        strncpy(&tcBuf[tcb], s, len);    // 見つからなかったので新規登録
        tcBuf[tcb + len] = 0;            // 終端文字コード
        ts[i] = &tcBuf[tcb];
        tl[i] = len;
        tcb += len + 1;
        tcs++;
        var[i] = strtol(ts[i], 0, 0);    // 定数だった場合に初期値を設定
    }

    return i;
}


// 変数名に使用できる文字かどうか判定する
int isAlphabetOrNumber(unsigned char c)
{
    if ('0' <= c && c <= '9') return 1;
    if ('a' <= c && c <= 'z') return 1;
    if ('A' <= c && c <= 'Z') return 1;
    if (c == '_') return 1;
    return 0;
}

// プログラムをトークンコード列に変換する
int lexer(String s, int tc[])
{
    int len;
    int i = 0;    // 今s[]のどこを呼んでいるか
    int j = 0;    // これまでに変換したトークン列の長さ

    while (1) {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r') {
            i++;
            continue;
        }

        if (s[i] == 0) {    // ファイル終端
            return j;
        }


        len = 0;
        if (strchr("(){}[];,", s[i]) != 0) {
            len = 1;

        } else if (isAlphabetOrNumber(s[i])) {
            while (isAlphabetOrNumber(s[i + len])) {
                len++;
            }

        } else if (strchr("=+-*/!%&~|<>?:.#", s[i]) != 0) {
            while (strchr("=+-*/!%&~|<>?:.#", s[i + len]) != 0 && s[i + len] != 0) {
                len++;
            }

        } else {
            printf("syntax error : %.10s\n", &s[i]);
            exit(1);
        }

        tc[j] = getTc(&s[i], len);
        i += len;
        j++;
    }
}

int tc[10000];


int main(int argc, const char **argv)
{
    int pc, pc1;
    unsigned char txt[10000];    // ソースコード用のバッファ
    
    loadText(argc, argv, txt, 10000);
    pc1 = lexer(txt, tc);
    tc[pc1] = tc[pc1 + 1] = tc[pc1 + 2] = tc[pc1 + 3] = getTc(".", 1);    // エラー表示のために末尾にピリオドを追加

    int semi = getTc(";", 1);
    for (pc = 0; pc < pc1; pc++) {
        if (tc[pc + 1] == getTc("=", 1) && tc[pc + 3] == semi) {
            var[tc[pc]] = var[tc[pc + 2]];    // 変数の単純代入

        } else if (tc[pc + 1] == getTc("=", 1) && tc[pc + 3] == getTc("+", 1) && tc[pc + 5] == semi) {
            var[tc[pc]] = var[tc[pc + 2]] + var[tc[pc + 4]];    // 足し算
        
        } else if (tc[pc + 1] == getTc("=", 1) && tc[pc + 3] == getTc("-", 1) && tc[pc + 5] == semi) {
            var[tc[pc]] = var[tc[pc + 2]] - var[tc[pc + 4]];    // 引き算
        
        } else if (tc[pc + 1] == getTc("=", 1) && tc[pc + 3] == getTc("*", 1) && tc[pc + 5] == semi) {
            var[tc[pc]] = var[tc[pc + 2]] * var[tc[pc + 4]];    // 掛け算
        
        } else if (tc[pc + 1] == getTc("=", 1) && tc[pc + 3] == getTc("/", 1) && tc[pc + 5] == semi) {
            var[tc[pc]] = var[tc[pc + 2]] / var[tc[pc + 4]];    // 割り算
        
        } else if (tc[pc] == getTc("print", 5) && tc[pc + 2] == semi) {
            printf("%d\n", var[tc[pc + 1]]);    // print
        
        } else {
            goto err;
        }

        while (tc[pc] != semi) pc++;
    }
    return 0;

err:
    printf("syntax error : %s %s %s %s\n", ts[tc[pc]], ts[tc[pc + 1]], ts[tc[pc + 2]], ts[tc[pc + 3]]);
    exit(1);
}