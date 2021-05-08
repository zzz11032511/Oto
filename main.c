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

//////////////////////////////////////////////////

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

//////////////////////////////////////////////////

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

enum { TcSemi = 0, TcDot, TcWiCard, Tc0, Tc1, Tc2, Tc3, Tc4, Tc5, Tc6, Tc7, Tc8, TcBrOpn, TcBrCls, TcSqBrOpn, TcSqBrCls, TcCrBrOpn, TcCrBrCls, 
    TcEEq, TcNEq, TcLt, TcGe, TcLe, TcGt, TcPlus, TcMinus, TcAster, TcSlash, TcPerce, TcAnd, TcShr, TcPlPlus, TcEqu,
    TcComma, TcExpr, TcExpr0, TcTmp0, TcTmp1, TcTmp2, TcTmp3, TcTmp4, TcTmp5, TcTmp6, TcTmp7, TcTmp8, TcTmp9 };
char tcInit[] = "; . !!* 0 1 2 3 4 5 6 7 8 ( ) [ ] { } == != < >= <= > + - * / % & >> ++ = , !!** !!*** _t0 _t1 _t2 _t3 _t4 _t5 _t6 _t7 _t8 _t9";

//////////////////////////////////////////////////

int phrCmp_tc[32 * 100];    // フレーズのlexer()結果を保存する
int ppc1;                   // 一致したフレーズの次のトークンをさす
int wpc[9];                 // ワイルドカードのトークンの場所をさす
int wpc1[9];                 // ワイルドカードのトークンの場所をさす

// tc[pc]からのトークンコード列がphrで指定されたトークン列と一致するかどうか調べる
// pid : フレーズID
//       phrCmp_tc[]のどこにphrのlexer結果をしまっておくかを決める
int phrCmp(int pid, String phr, int pc)
{
    int i0 = pid * 32;
    int i, i1, j, k, t;

    if (phrCmp_tc[i0 + 31] == 0) {
        i1 = lexer(phr, &phrCmp_tc[i0]);
        phrCmp_tc[i0 + 31] = i1;
    }
    i1 = phrCmp_tc[i0 + 31];
    for (i = 0; i < i1; i++) {
        t = phrCmp_tc[i0 + i];
        if (t == TcWiCard || t == TcExpr || t == TcExpr0) {
            i++;
            j = phrCmp_tc[i0 + i] - Tc0;    // 後続の番号を取得
            wpc[j] = pc;
            if (t == TcWiCard) {
                pc++;
                continue;                 
            }
            k = 0;    // ネストの深さ
            while (1) {
                if (tc[pc] == TcSemi) break;
                if (tc[pc] == TcComma && k == 0) break;
                if (tc[pc] == TcBrOpn || tc[pc] == TcSqBrOpn) k++;
                if (tc[pc] == TcBrCls || tc[pc] == TcSqBrCls) k--;
                if (k < 0) break;
                pc++;
            }
            wpc1[j] = pc;
            if (t == TcExpr && wpc[j] == pc) return 0;    // "!!**"では、長さ0は不一致とする.
            if (k > 0) return 0;    // カッコの深さがおかしい時も不一致とする.
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

//////////////////////////////////////////////////

typedef int *IntP;

enum { OpCpy = 0, OpCeq, OpCne, OpClt, OpCge, OpCle, OpCgt, OpAdd, OpSub, OpMul, OpDiv, OpMod, OpAnd, OpShr,
    OpAdd1, OpNeg, OpGoto, OpJeq, OpJne, OpJlt, OpJge, OpJle, OpJgt, OpLop, OpPrint, OpTime, OpEnd};

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

//////////////////////////////////////////////////

char tmp_flag[10];    // 一時変数の利用状況を管理

int tmpAlloc()
{
    int i;
    for (i = 0; i < 10; i++) {
        if (tmp_flag[i] == 0) break;
    }
    if (i >= 10) {
        printf("tmpAlloc: error\n");
        return -1;
    }
    tmp_flag[i] = 1;
    return i + TcTmp0;
}

void tmpFree(int i)
{
    if (TcTmp0 <= i && i <= TcTmp9) {
        tmp_flag[i - TcTmp0] = 0;
    }
}

//////////////////////////////////////////////////

int epc, epc1;

int exprSub(int priority);
int expr(int j);

int exprSub1(int i, int priority, int op)
{
    int j, k;
    epc++;
    j = exprSub(priority);
    k = tmpAlloc();
    putIc(op, &var[k], &var[i], &var[j], 0);
    tmpFree(i);
    tmpFree(j);
    if (i < 0 || j < 0) return -1;
    return k;
}

int exprSub(int priority)
{
    int i = -1;    // ここまでの計算結果が入っている変数のトークンコード番号
    int e0 = 0;
    ppc1 = 0;
    if (phrCmp(99, "( !!**0 )", epc)) {
        i = expr(0);
    
    } else if (tc[epc] == TcPlPlus) {
        epc++;
        i = exprSub(2);
        putIc(OpAdd1, &var[i], 0, 0, 0);
    
    } else if (tc[epc] == TcMinus) {
        epc++;
        e0 = exprSub(2);
        i = tmpAlloc();
        putIc(OpNeg, &var[i], &var[e0], 0, 0);

    } else {    // 変数もしくは定数
        i = tc[epc];
        epc++;
    }

    if (ppc1 > 0)
        epc = ppc1;
    
    while (1) {
        tmpFree(e0);
        if (i < 0 || e0 < 0) return -1;
        if (epc >= epc1) break;
        e0 = 0;
        if (tc[epc] == TcPlPlus) {
            epc++;
            e0 = i;
            i = tmpAlloc();
            putIc(OpCpy,  &var[i], &var[e0], 0, 0);
            putIc(OpAdd1, &var[e0], 0, 0, 0);
        
        } else if (TcAster <= tc[epc] && tc[epc] <= TcPerce && priority >= 4) {
            i = exprSub1(i, 3, tc[epc] - TcAster + OpMul);

        } else if (TcPlus <= tc[epc] && tc[epc] <= TcMinus && priority >= 5) {
            i = exprSub1(i, 4, tc[epc] - TcPlus + OpAdd);
            
        } else if (tc[epc] == TcShr && priority >= 6) {
            i = exprSub1(i, 5, OpShr);
            
        } else if (TcLt <= tc[epc] && tc[epc] <= TcGt && priority >= 7) {
            i = exprSub1(i, 6, tc[epc] - TcLt + OpClt);
            
        } else if (TcEEq <= tc[epc] && tc[epc] <= TcNEq && priority >= 8) {
            i = exprSub1(i, 7, tc[epc] - TcEEq + OpCeq);
            
        } else if (tc[epc] == TcAnd && priority >= 9) {
            i = exprSub1(i, 8, OpAnd);
            
        } else if (tc[epc] == TcEqu && priority >= 15) {
            epc++;
            e0 = exprSub(15);
            putIc(OpCpy, &var[i], &var[e0], 0, 0);

        } else 
            break;
    }
    return i;
}

int expr(int j)
{
    int i, k;
    int old_epc = epc;
    int old_epc1 = epc1;
    int s[19];

    if (wpc[j] == wpc1[j]) return 0;
    for (k = 0; k < 9; k++) {    // wpc[], wpc1[]を保存する
        s[k] = wpc[k];
        s[k + 9] = wpc1[k];
    }
    s[18] = ppc1;    // ppc1を保存する

    // epc, epc1を準備してexprSub()を呼び出す
    epc  = wpc[j];
    epc1 = wpc1[j];
    i = exprSub(99);

    if (epc < epc1) return -1;

    // 保存しておいた変数を全て復元する
    epc  = old_epc;
    epc1 = old_epc1;
    for (k = 0; k < 9; k++) {
        wpc[k] = s[k];
        wpc1[k] = s[k + 9];
    }
    ppc1 = s[18];
    return i;
}

///////////////////////////////////////////////////////////////////////////////

// 与えられた文字列をプログラムだと解釈して、内部コードを生成しic[]に出力する
int compile(String s)
{
    int pc, pc1, i;
    IntP *icq1;

    pc1 = lexer(s, tc);
    tc[pc1++] = TcSemi;    // 末尾に「;」を付け忘れることが多いので、付けてあげる.
    tc[pc1] = tc[pc1 + 1] = tc[pc1 + 2] = tc[pc1 + 3] = TcDot;    // エラー表示用のために末尾にピリオドを登録しておく.

    icq = ic;    // これでicqはic[0]を指すようになる。ここから書き始める
    for (i = 0; i < 10; i++) {    // 一時変数をすべて未使用にする
        tmp_flag[i] = 0;
    }

    for (pc = 0; pc < pc1;) {
        int e0 = 0;

        if (phrCmp(1, "!!*0 = !!*1;", pc)) {
            putIc(OpCpy, &var[tc[wpc[0]]], &var[tc[wpc[1]]], 0, 0);    // 単純代入

        } else if (phrCmp(9, "!!*0 = !!*1 + 1; if (!!*2 < !!*3) goto !!*4;", pc) && tc[wpc[0]] == tc[wpc[1]] && tc[wpc[0]] == tc[wpc[2]]) {
            putIc(OpLop, &var[tc[wpc[4]]], &var[tc[wpc[0]]], &var[tc[wpc[3]]], 0);

        } else if (phrCmp(2, "!!*0 = !!*1 + 1;", pc) && tc[wpc[0]] == tc[wpc[1]]) {
            putIc(OpAdd1, &var[tc[wpc[0]]], 0, 0, 0);    // 1の加算

        } else if (phrCmp(2, "!!*0 = !!*1 + !!*2;", pc)) {
            putIc(OpAdd, &var[tc[wpc[0]]], &var[tc[wpc[1]]], &var[tc[wpc[2]]], 0);    // 足し算
        
        } else if (phrCmp(3, "!!*0 = !!*1 - !!*2;", pc)) {
            putIc(OpSub, &var[tc[wpc[0]]], &var[tc[wpc[1]]], &var[tc[wpc[2]]], 0);    // 引き算
        
        } else if (phrCmp(4, "print !!**0;", pc)) {
            e0 = expr(0);
            putIc(OpPrint, &var[e0], 0, 0, 0);    // print
        
        } else if (phrCmp(0, "!!*0:", pc)) {    // ラベル定義命令
            var[tc[wpc[0]]] = icq - ic;    // ラベルに対応するicqを記録しておく

        } else if (phrCmp(5, "goto !!*0;", pc)) {
            putIc(OpGoto, &var[tc[wpc[0]]], 0, 0, 0);    // goto

        } else if (phrCmp(6, "if (!!*0 !!*1 !!*2) goto !!*3;", pc) && TcEEq <= tc[wpc[1]] && tc [wpc[1]] <= TcLt) {
            putIc(OpJeq + (tc[wpc[1]] - TcEEq), &var[tc[wpc[3]]], &var[tc[wpc[0]]], &var[tc[wpc[2]]], 0); 

        } else if (phrCmp(7, "time;", pc)) {
            putIc(OpTime, 0, 0, 0, 0);
            
        } else if (phrCmp(8, "!!***0;", pc)) {
            e0 = expr(0);

        } else {
            goto err;
        
        }
        tmpFree(e0);
        if (e0 < 0) goto err;
        pc = ppc1;
    }
    putIc(OpEnd, 0, 0, 0, 0);
    icq1 = icq;
    for (icq = ic; icq < icq1; icq += 5) {    // goto先の設定
        i = (int)icq[0];
        if (OpGoto <= i && i <= OpLop) {
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
    int i;
    while (1) {
        switch ((int) icp[0]) {
        case OpNeg:     *icp[1] = - *icp[2];          icp += 5; continue;
        case OpAdd1:    (*icp[1])++;                  icp += 5; continue;
        case OpMul:     *icp[1] = *icp[2] *  *icp[3]; icp += 5; continue;
        case OpDiv:     *icp[1] = *icp[2] /  *icp[3]; icp += 5; continue;
        case OpMod:     *icp[1] = *icp[2] %  *icp[3]; icp += 5; continue;
        case OpAdd:     *icp[1] = *icp[2] +  *icp[3]; icp += 5; continue;
        case OpSub:     *icp[1] = *icp[2] -  *icp[3]; icp += 5; continue;
        case OpShr:     *icp[1] = *icp[2] >> *icp[3]; icp += 5; continue;
        case OpClt:     *icp[1] = *icp[2] <  *icp[3]; icp += 5; continue;
        case OpCge:     *icp[1] = *icp[2] >= *icp[3]; icp += 5; continue;
        case OpCle:     *icp[1] = *icp[2] <= *icp[3]; icp += 5; continue;
        case OpCgt:     *icp[1] = *icp[2] >  *icp[3]; icp += 5; continue;
        case OpCeq:     *icp[1] = *icp[2] == *icp[3]; icp += 5; continue;
        case OpCne:     *icp[1] = *icp[2] != *icp[3]; icp += 5; continue;
        case OpAnd:     *icp[1] = *icp[2] &  *icp[3]; icp += 5; continue;
        case OpCpy:     *icp[1] = *icp[2];            icp += 5; continue;
        case OpPrint:
            printf("%d\n", *icp[1]);
            icp += 5;
            continue;
        case OpGoto:                           icp = (IntP *) icp[1]; continue;
        case OpJeq:  if (*icp[2] == *icp[3]) { icp = (IntP *) icp[1]; continue; } icp += 5; continue;
        case OpJne:  if (*icp[2] != *icp[3]) { icp = (IntP *) icp[1]; continue; } icp += 5; continue;
        case OpJlt:  if (*icp[2] <  *icp[3]) { icp = (IntP *) icp[1]; continue; } icp += 5; continue;
        case OpTime:
            printf("time: %.3f[sec]\n", (clock() - t0) / (double) CLOCKS_PER_SEC);
            icp += 5;
            continue;
        case OpEnd:
            return;
        case OpLop:
            i = *icp[2];
            i++;
            *icp[2] = i;
            if (i < *icp[3]) {
                icp = (IntP *) icp[1];
                continue;
            }
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