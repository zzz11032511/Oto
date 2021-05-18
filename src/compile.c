#include "util.h"
#include "lexer.h"
#include "compile.h"

int phrCmp_tc[32 * 100];    // フレーズのlexer()結果を保存する
int ppc1;                   // 一致したフレーズの次のトークンをさす
int wpc[9];                 // ワイルドカードのトークンの場所をさす
int wpc1[9];                // ワイルドカードのトークンの場所をさす

/**
 *  tc[pc]からのトークンコード列がphrで指定されたトークン列と一致するかどうかを調べる
 * example :
 *      phrCmp(1, "!!*0 = !!*1;", pc);
 */
int phrCmp(int pid, String phr, int pc, int *tc, int *var)
{
    int i0 = pid * 32;
    int i, i1, j, k, t;

    if (phrCmp_tc[i0 + 31] == 0) {
        i1 = lexer(phr, &phrCmp_tc[i0], var);
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
            if (t == TcExpr && wpc[j] == pc) return 0;     // "!!**"では、長さ0は不一致とする
            if (k > 0) return 0;    // 括弧の深さがおかしい時も不一致とする
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


char tmp_flag[10];    // 一時変数の利用状況を管理

// 未使用の一時変数の確保
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

// iが一時変数を指すトークンコードであれば解放する
void tmpFree(int i)
{
    if (TcTmp0 <= i && i <= TcTmp9) {
        tmp_flag[i - TcTmp0] = 0;
    }
}


int epc, epc1;    // expr()のためのプログラムカウンタ

int exprSub(int priority, int *tc, int *var);
int expr(int j, int *tc, int *var);

/* 二項演算子処理 */
int exprSub1(int i, int priority, int op, int *tc, int *var)
{
    int j, k;
    epc++;
    j = exprSub(priority, tc, var);
    k = tmpAlloc();
    putIc(op, &var[k], &var[i], &var[j], 0);
    tmpFree(i);
    tmpFree(j);
    if (i < 0 || j < 0) return -1;
    return k;
}

/* 式の解釈の主要部分 */
int exprSub(int priority, int *tc, int *var)
{
    int i = -1;    // ここまでの計算結果が入っている変数のトークンコード番号
    int e0 = 0;
    ppc1 = 0;
    if (phrCmp(99, "( !!**0 )", epc, tc, var)) {
        i = expr(0, tc, var);
    
    } else if (tc[epc] == TcPlPlus) {
        epc++;
        i = exprSub(2, tc, var);
        putIc(OpAdd1, &var[i], 0, 0, 0);
    
    } else if (tc[epc] == TcMinus) {
        epc++;
        e0 = exprSub(2, tc, var);
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
            i = exprSub1(i, 3, tc[epc] - TcAster + OpMul, tc, var);

        } else if (TcPlus <= tc[epc] && tc[epc] <= TcMinus && priority >= 5) {
            i = exprSub1(i, 4, tc[epc] - TcPlus + OpAdd, tc, var);
            
        } else if (TcLt <= tc[epc] && tc[epc] <= TcGt && priority >= 7) {
            i = exprSub1(i, 5, tc[epc] - TcLt + OpClt, tc, var);
            
        } else if (TcEEq <= tc[epc] && tc[epc] <= TcNEq && priority >= 8) {
            i = exprSub1(i, 6, tc[epc] - TcEEq + OpCeq, tc, var);
            
        } else if (tc[epc] == TcEqu && priority >= 15) {
            epc++;
            e0 = exprSub(15, tc, var);
            putIc(OpCpy, &var[i], &var[e0], 0, 0);

        } else 
            break;
    }
    return i;
}

// 式の解釈処理
int expr(int j, int *tc, int *var)
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
    i = exprSub(99, tc, var);

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


int *ic[10000];    // 内部コード
int **icq;         // ic[]への書き込みポインタ

int compile(String s, int *tc, int *var)
{
    int pc;     // プログラムカウンタ
    int end;    // プログラムの最後
    int **icq1;

    end = lexer(s, tc, var);    // まずsをトークンコード列に変換する
    tc[end++] = TcSemi;    // 末尾に「;」を付け忘れることが多いので、付けてあげる.
    tc[end] = tc[end + 1] = tc[end + 2] = tc[end + 3] = TcDot;    // エラー表示用のために末尾にピリオドを登録しておく.

    icq = ic;    // これでicqはic[0]を指すようになる。ここから書き始める
    for (int i = 0; i < 10; i++) {    // 一時変数をすべて未使用にする
        tmp_flag[i] = 0;
    }

    for (pc = 0; pc < end;) {
        int e0 = 0;

        if (phrCmp(1, "!!*0 = !!*1;", pc, tc, var)) {
            putIc(OpCpy, &var[tc[wpc[0]]], &var[tc[wpc[1]]], 0, 0);    // 単純代入

        } else if (phrCmp(9, "!!*0 = !!*1 + 1; if (!!*2 < !!*3) goto !!*4;", pc, tc, var) && tc[wpc[0]] == tc[wpc[1]] && tc[wpc[0]] == tc[wpc[2]]) {
            putIc(OpLop, &var[tc[wpc[4]]], &var[tc[wpc[0]]], &var[tc[wpc[3]]], 0);

        } else if (phrCmp(2, "!!*0 = !!*1 + 1;", pc, tc, var) && tc[wpc[0]] == tc[wpc[1]]) {
            putIc(OpAdd1, &var[tc[wpc[0]]], 0, 0, 0);    // 1の加算

        } else if (phrCmp(2, "!!*0 = !!*1 + !!*2;", pc, tc, var)) {
            putIc(OpAdd, &var[tc[wpc[0]]], &var[tc[wpc[1]]], &var[tc[wpc[2]]], 0);    // 足し算
        
        } else if (phrCmp(3, "!!*0 = !!*1 - !!*2;", pc, tc, var)) {
            putIc(OpSub, &var[tc[wpc[0]]], &var[tc[wpc[1]]], &var[tc[wpc[2]]], 0);    // 引き算
        
        } else if (phrCmp(4, "print !!**0;", pc, tc, var)) {
            e0 = expr(0, tc, var);
            putIc(OpPrint, &var[e0], 0, 0, 0);    // print
        
        } else if (phrCmp(0, "!!*0:", pc, tc, var)) {    // ラベル定義命令
            var[tc[wpc[0]]] = icq - ic;    // ラベルに対応するicqを記録しておく

        } else if (phrCmp(5, "goto !!*0;", pc, tc, var)) {
            putIc(OpGoto, &var[tc[wpc[0]]], 0, 0, 0);    // goto

        } else if (phrCmp(6, "if (!!*0 !!*1 !!*2) goto !!*3;", pc, tc, var) && TcEEq <= tc[wpc[1]] && tc [wpc[1]] <= TcLt) {
            putIc(OpJeq + (tc[wpc[1]] - TcEEq), &var[tc[wpc[3]]], &var[tc[wpc[0]]], &var[tc[wpc[2]]], 0); 

        } else if (phrCmp(7, "time;", pc, tc, var)) {
            putIc(OpTime, 0, 0, 0, 0);
            
        } else if (phrCmp(8, "!!***0;", pc, tc, var)) {
            e0 = expr(0, tc, var);

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
    printf("syntax error : %s %s %s %s\n", ts[tc[pc]], ts[tc[pc + 1]], ts[tc[pc + 2]], ts[tc[pc + 3]]);
    return 1;
}