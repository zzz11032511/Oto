#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"
#include "compile.h"

int tmpVpc[5];    // putIc()で指定するTcを入れる場所
int vp = 0;

/* 引数に渡されたトークンのパターンと実際のコードが一致しているかを調べる関数 */
int ptnCmp(tokenBuf_t *tcBuf, int *pc, int pattern, ...)
{
    va_list ap;
    va_start(ap, pattern);    // 可変長引数

    int ppc = *pc;    // 最初のpcを保存しておく
    int ptnTc = pattern;    // パターンから読み込んだトークン
    int nest = 0;    // ネストの深さ

    while (1) {
        // ネストの処理をいつか書く
        int tc = tcBuf->tc[*pc];
        // printf("tc : %d, ptnTc : %d\n", tc, ptnTc);

        if (ptnTc == TcSemi && tc == TcSemi) {
            // セミコロンなら終わり
            (*pc)++;    // 進めとく
            break;
        }

        // (){}[]のときの処理
        if ((tc == TcBrOpn) || (tc == TcSqBrOpn) || (tc == TcCrBrOpn)) {
            nest++;
            (*pc)++;
            continue;

        } else if ((tc == TcBrCls) || (tc == TcSqBrCls) || (tc == TcCrBrCls)) {
            nest--;
            (*pc)++;
            continue;
        }

        if (tc == ptnTc) {
            // 既にあるトークンと一致した

        } else if (ptnTc == TcType && (TcInt <= tc && tc <= TcFloat)) {
            // 変数の型

        } else if (ptnTc == TcIdentifier && tc > TcEnd) {
            // 変数名, 識別子の時の処理
            tmpVpc[vp++] = tc;

        } else if (ptnTc == TcConst && tc > TcEnd) {
            // 定数の時の処理
            tmpVpc[vp++] = tc;

        } else if (ptnTc == TcExpr) {
            // 式の時の処理
            (*pc) = ppc;    // 式と思われるときはpcを元に戻して返す
            vp = 0;         // 違うときはvpも元に戻す
            return 1;

        } else {
            (*pc) = ppc;    // もし一致しないときはpcをptnCmp()呼び出し前に戻す
            vp = 0;
            return 0;    // 一致しなかったので0を返す
        }
        ptnTc = va_arg(ap, int);    // 次のトークンパータンを参照
        (*pc)++;
    }

    if (nest != 0) {
        vp = 0;
        return 0;    // ネストが正常でないので不一致
    }

    va_end(ap);

    return 1;
}


/**
 * ic[]に書き込むための関数
 * 
 * args:
 *      ic      : 内部コード列
 *      icp     : 現在指しているicのインデックス
 *                中で加算したりするのでポインタを渡してね
 *      op      : 処理を表す内部コード
 *      v1 ~ v4 : 渡す値(ポインタ)
 */
void putIc(int **ic, int *icp, int op, int *v1, int *v2, int *v3, int *v4)
{
    ic[(*icp)++] = (int *)op;
    ic[(*icp)++] = v1;
    ic[(*icp)++] = v2;
    ic[(*icp)++] = v3;
    ic[(*icp)++] = v4;
}

/* 内部コードに変換する関数 */
int compile(String s, tokenBuf_t *tcBuf, int *var, int **ic)
{
    int pc, pc1;    // プログラムカウンタ

    pc1 = lexer(s, tcBuf, var);

    int *tc = tcBuf->tc;

    // デバッグ用, tcの表示
    printf("tc : ");
    for (pc = 0; pc < pc1; pc++) {
        printf("%d ", tcBuf->tc[pc]);
    }
    printf("\n");

    int icp = 0;    // icをどこまで書き込んだか
    int ppc = 0;    // ptnCmp()前のpcの値を保存しておく
    pc = 0;
    while(pc < pc1) {
        // printf("pc : %d\n", pc);
        ppc = pc;

        if (ptnCmp(tcBuf, &pc, TcType, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            /* <type> <identifier> = <const>; (変数宣言) */
            printf("<type> <identifier> = <const>;\n");


        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            /* <identifier> = <const>; (単純代入) */
            printf("<identifier> = <const>;\n");

        } else if (ptnCmp(tcBuf, &pc, TcConst, TcPlus,  TcConst, TcSemi)) {
            /* <const> + <const>; (加算) */
            printf("<const> + <const>;\n");
            putIc(ic, &icp, OpAdd, &var[tmpVpc[0]], &var[tmpVpc[1]], 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcConst, TcMinus,  TcConst, TcSemi)) {
            /* <const> - <const>; (引算) */
            printf("<const> - <const>;\n");
            putIc(ic, &icp, OpSub, &var[tmpVpc[0]], &var[tmpVpc[1]], 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcConst, TcAster,  TcConst, TcSemi)) {
            /* <const> * <const>; (掛算) */
            printf("<const> * <const>;\n");
            putIc(ic, &icp, OpMul, &var[tmpVpc[0]], &var[tmpVpc[1]], 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcConst, TcSlash,  TcConst, TcSemi)) {
            /* <const> / <const>; (割算) */
            printf("<const> / <const>;\n");
            putIc(ic, &icp, OpDiv, &var[tmpVpc[0]], &var[tmpVpc[1]], 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcPlus,  TcConst, TcSemi)) {
            /* <identifier> = <const> + <const>; (加算) */
            printf("<identifier> = <const> + <const>;\n");


        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcMinus, TcConst, TcSemi)) {
            /* <identifier> = <const> - <const>; (減算) */
            printf("<identifier> = <const> - <const>;\n");

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcAster, TcConst, TcSemi)) {
            /* <identifier> = <const> * <const>; (掛算) */
            printf("<identifier> = <const> * <const>;\n");

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcSlash, TcConst, TcSemi)) {
            /* <identifier> = <const> / <const>; (割算) */
            printf("<identifier> = <const> / <const>;\n");

        } else if (ptnCmp(tcBuf, &pc, TcExpr)) {
            /* <expr>; (算術式) */
            printf("<expr>;\n");

        } else {
            goto err;
        }
    }

    return 0;

// エラー表示
err:
    fprintf(stderr, "compile error\n");
    return 1;
}