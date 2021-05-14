#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lexer.h"
#include "tmpVal.h"
#include "compile.h"

int phrCmp_tc[32 * 100];    // フレーズのlexer()結果を保存する
int ppc1;                   // 一致したフレーズの次のトークンをさす
int wpc[9];                 // ワイルドカードのトークンの場所をさす
int wpc1[9];                // ワイルドカードのトークンの場所をさす

/**
 * tc[pc]からのトークンコード列がphrで指定されたトークン列と一致するかどうか調べる
 * 
 * pid : フレーズID
 *       phrCmp_tc[]のどこにphrのlexer結果をしまっておくかを決める
 */
int phrCmp(int pid, const char *phr, int pc)
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


#define IC_SIZE 10000

int *ic[IC_SIZE];    // 内部コード
int **icq;         // ic[]への書き込みカウンタ

/**
 * 内部コードに書き込むための便利関数
 */
void putIc(int op, int *p1, int *p2, int *p3, int *p4)
{
    icq[0] = &op;
    icq[1] = p1;
    icq[2] = p2;
    icq[3] = p3;
    icq[4] = p4;
    icq += 5;
}

int compile(unsigned char *s, int *tc)
{
    int pc, pc1;    // プログラムカウンタ
    int *icq1;

    pc1 = lexer(s, tc);
    tc[pc1++] = TcSemi;    // 末尾の「;」を付けてあげる
    tc[pc1] = tc[pc1 + 1] = tc[pc1 + 2] = tc[pc1 + 3] = TcDot;  // エラー表示用のために末尾にピリオドを追加

    icq = ic;
    for (int i = 0; i < 10; i++) {    // 一時変数をすべて未使用にする
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
        int i = (int)icq[0];
        if (OpGoto <= i && i <= OpLop) {
            icq[1] = (int *)(*icq[1] + ic);
        }
    }
    return icq1 - ic;

err:
    // エラー処理
    printf("syntax error : %s %s %s %s\n", ts[tc[pc]], ts[tc[pc + 1]], ts[tc[pc + 2]], ts[tc[pc + 3]]);
    return 1;    
}