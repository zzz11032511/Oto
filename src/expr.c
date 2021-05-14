#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lexer.h"
#include "tmpVal.h"
#include "compile.h"

int epc, epc1;    // expr()のためのプログラムカウンタ

int exprSub(int priority);
int expr(int j);

/**
 * 二項演算子処理
 */
int exprSub1(int i, int priority, int op)
{
    int j, k;
    
    epc++;
    j = exprSub(priority);
    k = tmpAlloc();
    putIc(op, &var[k], &var[i], &var[j], 0);
    tmpFree(i);
    tmpFree(j);
    
    if (i < 0 || j < 0)
        return -1;
    
    return k;
}

// 式の解釈の主要部分
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

// 式の解釈処理
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