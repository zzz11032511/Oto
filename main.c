#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned char *String;

// コマンドライン引数で指定されたソースファイルをtに読み込む
int loadText(String path, String t, int siz)
{
    FILE *fp;
    unsigned char s[1000];
    int i = 0, j;

    if (path[0] == 34) {
        i = 1;
    }
    
    for (j = 0; ; j++) {
        if (path[i + j] == 0 || path[j + i] == 34) {
            break;    // ファイル名の末尾に来たらそこで終わり
        }
        s[j] = path[i + j]; 
    }
    s[j] = 0;

    fp = fopen(s, "rt");
    if (fp == 0) {
        printf("fopen error : %s\n", path);
        return 1;    // 失敗
    }
    i = fread(t, 1, siz - 1, fp);
    
    fclose(fp);
    t[i] = 0;    // 終端マークを書いておく
    return 0;
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
            while (isAlphabetOrNumber(s[i + len])) len++;

        } else if (strchr("=+-*/!%&~|<>?:.#", s[i]) != 0) {
            while (strchr("=+-*/!%&~|<>?:.#", s[i + len]) != 0 && s[i + len] != 0) len++;

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


// プログラム実行
int run(String s)
{
    clock_t t0 = clock();
    int pc, pc1;

    pc1 = lexer(s, tc);
    tc[pc1++] = getTc(";", 1);    // 末尾に;をつけてあげる
    tc[pc1] = tc[pc1 + 1] = tc[pc1 + 3] = getTc(".", 1);    // エラー表示のために末尾にピリオドを追加

    int semi = getTc(";", 1);
    for (pc = 0; pc < pc1; pc++) {    // ラベル定義命令を探して位置を登録
        if (tc[pc + 1] == getTc(":", 1)) {
            var[tc[pc]] = pc + 2;    // ラベル定義命令の次のpc値を変数に記憶させておく
        }
    }

    for (pc = 0; pc < pc1;) {
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
        
        } else if (tc[pc + 1] == getTc(":", 1)) {    // ラベル定義命令
            pc += 2;    // 読み飛ばす
            continue;
        
        } else if (tc[pc] == getTc("goto", 4) && tc[pc + 2] == semi) {
            pc = var[tc[pc + 1]];    // goto
            continue;

        } else if (tc[pc] == getTc("if", 2) && tc[pc + 1] == getTc("(", 1) && tc[pc + 5] == getTc(")", 1) && tc[pc + 6] == getTc("goto", 4) && tc[pc + 8] == semi) {
            int gpc = var[tc[pc + 7]];
            int v0  = var[tc[pc + 2]];
            int v1  = var[tc[pc + 4]];
            
            // if (...) goto
            if (tc[pc + 3] == getTc("!=", 2) && v0 != v1) {
                pc = gpc;
                continue;
            }
            if (tc[pc + 3] == getTc("==", 2) && v0 == v1) {
                pc = gpc;
                continue;
            }
            if (tc[pc + 3] == getTc("<", 1) && v0 < v1) {
                pc = gpc;
                continue;
            }

        } else if (tc[pc] == getTc("time", 4) && tc[pc + 1] == semi) {
            printf("time: %.3f[sec]\n", clock() / (double)CLOCKS_PER_SEC);

        } else if (tc[pc] == semi) {
            // 何もしない

        } else {
            goto err;
        
        }

        while (tc[pc] != semi) pc++;
        pc++;
    }
    return 0;

err:
    printf("syntax error : %s %s %s %s\n", ts[tc[pc]], ts[tc[pc + 1]], ts[tc[pc + 2]], ts[tc[pc + 3]]);
    return 1;
}


int main(int argc, const char **argv)
{
    unsigned char txt[10000];
    int i;

    if (argc >= 2) {
        if (loadText((String)argv[1], txt, 10000) == 0) {
            run(txt);
        }
        exit(0);
    }

    while (1) {    // Read-Eval-Print Loop
        printf("\n>");

        fgets(txt, 10000, stdin);
        i = strlen(txt);
        if (txt[i - 1] == '\n') {
            txt[i - 1] = 0;    // 改行コードを消す
        }

        if (strncmp(txt, "run ", 4) == 0) {
            if (loadText(&txt[4], txt, 10000) == 0) {
                run(txt);
            }
        } else if (strcmp(txt, "exit") == 0) {
            exit(0);
        } else {
            run(txt);
        }
    }
}