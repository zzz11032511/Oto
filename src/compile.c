#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"
#include "expr.h"
#include "ic.h"


int32_t tVpc[5];    // putIc()で指定するTcを入れる場所(Temp Var Pcのつもり)
int32_t vp = 0;     // tVpcへのポインタ

/* 引数に渡されたトークンのパターンと実際のコードが一致しているかを調べる関数 */
int32_t ptnCmp(tokenBuf_t *tcBuf, int32_t *pc, int32_t pattern, ...)
{
    va_list ap;
    va_start(ap, pattern);    // 可変長引数

    int32_t ppc = *pc;    // 最初のpcを保存しておく
    int32_t ptnTc = pattern;    // パターンから読み込んだトークン
    int32_t nest = 0;    // ネストの深さ

    while (1) {
        // ネストの処理をいつか書く
        int32_t tc = tcBuf->tc[*pc];
        // print32_tf("tc : %d, ptnTc : %d\n", tc, ptnTc);

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

        } else if (ptnTc == TcType && (Tcint32_t <= tc && tc <= TcFloat)) {
            // 変数の型

        } else if (ptnTc == TcIdentifier && tc > TcEnd) {
            // 変数名, 識別子の時の処理
            tVpc[vp++] = tc;

        } else if (ptnTc == TcConst && tc > TcEnd) {
            // 定数の時の処理
            tVpc[vp++] = tc;

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
        ptnTc = va_arg(ap, int32_t);    // 次のトークンパータンを参照
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
void putIc(int32_t **ic, int32_t *icp, int32_t op, int32_t *v1, int32_t *v2, int32_t *v3, int32_t *v4)
{
    // printtf("pc : %d | op : %d, v1 : %d, v2 : %d, v3 : %d, v4 : %d\n", *icp, op, v1, v2, v3, v4);
    ic[(*icp)++] = (int32_t *)op;
    ic[(*icp)++] = v1;
    ic[(*icp)++] = v2;
    ic[(*icp)++] = v3;
    ic[(*icp)++] = v4;
}

/* 内部コードに変換する関数 */
int32_t compile(String s, tokenBuf_t *tcBuf, var_t **var, int32_t **ic)
{
    int32_t pc, pc1;    // プログラムカウンタ

    pc1 = lexer(s, tcBuf, var);

    int32_t *tc = tcBuf->tc;

    // デバッグ用, tcの表示
    printf("tc : ");
    for (pc = 0; pc < pc1; pc++) {
        printf("%d ", tcBuf->tc[pc]);
    }
    printf("\n");

    int32_t icp = 0;    // icをどこまで書き込んだか
    int32_t ppc = 0;    // ptnCmp()前のpcの値を保存しておく
    pc = 0;
    while(pc < pc1) {
        // print32_tf("pc : %d\n", pc);
        ppc = pc;

        if (ptnCmp(tcBuf, &pc, TcType, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            /* <type> <identifier> = <const>; (変数宣言) */
            printf("<type> <identifier> = <const>;\n");

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            /* <identifier> = <const>; (単純代入) */
            printf("<identifier> = <expr>;\n");
            putIc(ic, &pc, OpCpy, var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcExpr)) {
            /* <expr>; (算術式) */
            printf("<expr>;\n");
            expr(tcBuf, &pc, var, ic);

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