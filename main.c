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

    if (path[0] == 34) {    // ダブルクオートを読み飛ばす
        i = 1;
    }

    // ファイル名取得    
    for (j = 0; ; j++) {
        if (path[i + j] == 0 || path[j + i] == 34) {
            break;    // ファイル名の末尾に来たらそこで終わり
        }
        s[j] = path[i + j]; 
    }
    s[j] = 0;

    // ファイルオープン
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


#define MAX_TC 255       // トークンコードの最大値
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

enum { TcSemi = 0, TcDot, TcWiCard, Tc0, Tc1, Tc2, Tc3, Tc4, Tc5, Tc6, Tc7, Tc8, TcEEq, TcNEq, TcLt, TcGe, TcLe, TcGt };
char tcInit[] = "; . !!* 0 1 2 3 4 5 6 7 8 == != < >= <= >";


int phrCmp_tc[32 * 100];    // フレーズのlexer()結果を保存する
int ppc1;                   // 一致したフレーズの次のトークンをさす
int wpc[9];                 // ワイルドカードのトークンの場所をさす

// tc[pc]からのトークンコード列がphrで指定されたトークン列と一致するかどうか調べる
// pid : フレーズID
//       phrCmp_tc[]のどこにphrのlexer結果をしまっておくかを決める
int phrCmp(int pid, String phr, int pc)
{
    int i0 = pid * 32;
    int i, i1, j;

    if (phrCmp_tc[i0 + 31] == 0) {
        i1 = lexer(phr, &phrCmp_tc[i0]);
        phrCmp_tc[i0 + 31] = i1;
    }
    i1 = phrCmp_tc[i0 + 31];
    for (i = 0; i < i1; i++) {
        if (phrCmp_tc[i0 + i] == TcWiCard) {
            i++;
            j = phrCmp_tc[i0 + i] - Tc0;    // 後続の番号を取得
            wpc[j] = pc;
            pc++;
            continue; 
        }
        if (phrCmp_tc[i0 + i] != tc[pc]) {    // マッチせず
            return 0;
        }
        pc++;
    }

    ppc1 = pc;
    return 1;
}


typedef int *IntP;

enum { OpCpy = 0, OpAdd, OpSub, OpPrint, OpGoto, OpJeq, OpJne, OpJlt, OpJge, OpJle, OpJgt, OpTime, OpEnd, OpAdd1 };

IntP ic[10000];    // 内部コード
IntP *icq;         // ic[]への書き込みポインタ

// ic[]へ簡単に書き込むための便利関数
void putIc(int op, IntP p1, IntP p2, IntP p3, IntP p4)
{
    icq[0] = (IntP)op;
    icq[1] = p1;
    icq[2] = p2;
    icq[3] = p3;
    icq[4] = p4;
    icq += 5;
}


// 与えられた文字列をプログラムだと解釈して、内部コードを生成しic[]に出力する
int compile(String s)
{
    int pc, pc1, i;
    IntP *icq1;

    pc1 = lexer(s, tc);
    tc[pc1++] = TcSemi;    // 末尾に「;」を付け忘れることが多いので、付けてあげる.
    tc[pc1] = tc[pc1 + 1] = tc[pc1 + 2] = tc[pc1 + 3] = TcDot;    // エラー表示用のために末尾にピリオドを登録しておく.

    icq = ic;    // これでicqはic[0]を指すようになる。ここから書き始める
    for (pc = 0; pc < pc1;) {
        if (phrCmp(1, "!!*0 = !!*1;", pc)) {
            putIc(OpCpy, &var[tc[wpc[0]]], &var[tc[wpc[1]]], 0, 0);    // 単純代入

        } else if (phrCmp(2, "!!*0 = !!*1 + 1;", pc) && tc[wpc[0]] == tc[wpc[1]]) {
            putIc(OpAdd1, &var[tc[wpc[0]]], 0, 0, 0);    // 1の加算

        } else if (phrCmp(2, "!!*0 = !!*1 + !!*2;", pc)) {
            putIc(OpAdd, &var[tc[wpc[0]]], &var[tc[wpc[1]]], &var[tc[wpc[2]]], 0);    // 足し算
        
        } else if (phrCmp(3, "!!*0 = !!*1 - !!*2;", pc)) {
            putIc(OpSub, &var[tc[wpc[0]]], &var[tc[wpc[1]]], &var[tc[wpc[2]]], 0);    // 引き算
        
        } else if (phrCmp(4, "print !!*0;", pc)) {
            putIc(OpPrint, &var[tc[wpc[0]]], 0, 0, 0);    // print
        
        } else if (phrCmp(0, "!!*0:", pc)) {    // ラベル定義命令
            var[tc[wpc[0]]] = icq - ic;    // ラベルに対応するicqを記録しておく

        } else if (phrCmp(5, "goto !!*0;", pc)) {
            putIc(OpGoto, &var[tc[wpc[0]]], 0, 0, 0);    // goto

        } else if (phrCmp(6, "if (!!*0 !!*1 !!*2) goto !!*3;", pc) && TcEEq <= tc[wpc[1]] && tc [wpc[1]] <= TcLt) {
            putIc(OpJeq + (tc[wpc[1]] - TcEEq), &var[tc[wpc[3]]], &var[tc[wpc[0]]], &var[tc[wpc[2]]], 0); 

        } else if (phrCmp(7, "time;", pc)) {
            putIc(OpTime, 0, 0, 0, 0);
            
        } else if (phrCmp(8, ";", pc)) {
            // 何もしない

        } else {
            goto err;
        
        }
        pc = ppc1;
    }
    putIc(OpEnd, 0, 0, 0, 0);
    icq1 = icq;
    for (icq = ic; icq < icq1; icq += 5) {    // goto先の設定
        i = (int)icq[0];
        if (OpGoto <= i && i <= OpJgt) {
            icq[1] = (IntP)(*icq[1] + ic);
        }
    }
    return icq1 - ic;
err:
    printf("syntax error : %s %s %s %s\n", ts[tc[pc]], ts[tc[pc + 1]], ts[tc[pc + 2]], ts[tc[pc + 3]]);
    return 1;
}

// ic[]に格納された内部コードを高速に実行する
void exec()
{
    clock_t t0 = clock();
    IntP *icp = ic;
    while (1) {
        switch ((int)icp[0]) {
            case OpCpy:
                *icp[1] = *icp[2];
                icp += 5;
                continue;

            case OpAdd:
                *icp[1] = *icp[2] + *icp[3];
                icp += 5;
                continue;
            
            case OpSub:
                *icp[1] = *icp[2] - *icp[3];
                icp += 5;
                continue;
            
            case OpPrint:
                printf("%d\n", *icp[1]);
                icp += 5;
                continue;
            
            case OpGoto:
                icp = (IntP *)icp[1];
                continue;

            case OpJeq:
                if (*icp[2] == *icp[3]) {
                    icp = (IntP *)icp[1];
                    continue;
                }
                icp += 5;
                continue;
            
            case OpJne:
                if (*icp[2] != *icp[3]) {
                    icp = (IntP *)icp[1];
                    continue;
                }
                icp += 5;
                continue;

            case OpJlt:
                if (*icp[2] < *icp[3]) {
                    icp = (IntP *)icp[1];
                    continue;
                }
                icp += 5;
                continue;

            case OpTime:
                printf("time: %.3f[sec]\n", (clock() - t0) / (double) CLOCKS_PER_SEC);
                icp += 5;
                continue;

            case OpEnd:
                return;

            case OpAdd1:
                (*icp[1])++;
                icp += 5;
                continue;
        }
    }
}

// プログラム実行
int run(String s)
{
    if (compile(s) < 0)
        return 1;
    exec();
    return 0;
}


int main(int argc, const char **argv)
{
    unsigned char txt[10000];
    int i;

    lexer(tcInit, tc);    // あらかじめtcInitをlexer()する

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