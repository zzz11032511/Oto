#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"
 
#define TcExpr         -1
#define TcType         -2
#define TcIdentifier   -3
#define TcConst        -4

/* 引数に渡されたトークンのパターンと実際のコードが一致しているかを調べる関数 */
int ptnCmp(tokenBuf_t *tcBuf, int *pc, int pattern, ...)
{
    va_list ap;

    va_start(ap, pattern);    // 可変長引数

    int ppc = *pc; // 最初のpcを保存しておく
    int ptnTc = pattern;    // パターンから読み込んだトークン
    while (1) {
        // ネストの処理をいつか書く
        int tc = tcBuf->tc[*pc];
        printf("tc : %d, ptnTc : %d\n", tc, ptnTc);

        if (ptnTc == TcSemi && tc == TcSemi) {
            // セミコロンなら終わり
            (*pc)++;    // 進めとく
            break;
        }

        if (tc == ptnTc) {
            // 既にあるトークンと一致した

        } else if (ptnTc == TcType && (TcInt <= tc && tc <= TcFloat)) {
            // 変数の型

        } else if (ptnTc == TcIdentifier && tc > TcEnd) {
            // 変数名, 識別子の時の処理

        } else if (ptnTc == TcConst && tc > TcEnd) {
            // 定数の時の処理

        } else if (ptnTc == TcExpr) {
            // 式の時の処理

        } else {
            (*pc) = ppc;    // もし一致しないときはpcをptnCmp()呼び出し前に戻す
            return 0;    // 一致しなかったので0を返す
        }
        ptnTc = va_arg(ap, int);    // 次のトークンパータンを参照
        (*pc)++;
    }
    va_end(ap);

    return 1;
}

/* 内部コードに変換する関数 */
int compile(String s, tokenBuf_t *tcBuf, var_t **var, int **ic)
{
    int pc, pc1;    // プログラムカウンタ

    pc1 = lexer(s, tcBuf);

    int pattern[20];    // 構文のパターン

    // デバッグ用, tcの表示
    printf("tc\n");
    for (pc = 0; pc < pc1; pc++) {
        printf("%d ", tcBuf->tc[pc]);
    }
    printf("\n");

    pc = 0;
    while(pc < pc1) {
        // printf("pc : %d\n", pc);

        if (ptnCmp(tcBuf, &pc, TcType, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            /* <type> <identifier> = <const>; (変数宣言) */
            printf("<type> <identifier> = <const>;\n");
        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            /* <identifier> = <const>; (単純代入) */
            printf("<identifier> = <const>;\n");
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
        } else {
            goto err;
        }
    }

    return 0;

// エラー表示
err:
    fprintf(stderr, "syntax error\n");
    return 1;
}