#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr.h"
#include "ic.h"
#include "lexer.h"
#include "token.h"
#include "util.h"
#include "variable.h"
#include "control.h"

int32_t tVpc[5];  // putIc()で指定するTcを入れる場所(Temp Var Pcのつもり)
int32_t vp = 0;  // tVpcへのポインタ

/* 引数に渡されたトークンのパターンと実際のコードが一致しているかを調べる関数
 */
int32_t ptnCmp(tokenBuf_t *tcBuf, int32_t *pc, int32_t pattern, ...) {
    va_list ap;
    va_start(ap, pattern);  // 可変長引数

    int32_t ppc   = *pc;      // 最初のpcを保存しておく
    int32_t ptnTc = pattern;  // パターンから読み込んだトークン
    int32_t nest  = 0;        // ネストの深さ

    while (1) {
        // ネストの処理をいつか書く
        int32_t tc = tcBuf->tc[*pc];
        printf("ptnTc : %d, tc : %d, pc : %d\n", ptnTc, tc, *pc);

        if (ptnTc == TcSemi && tc == TcSemi) {
            // セミコロンなら終わり
            (*pc)++;  // 進めとく
            break;
        } 
        if (ptnTc == TcStop) {
            (*pc) = ppc;  // TcStopのとき(if, whileなど)はpcを元に戻して返す        
            vp    = 0;  // 違うときはvpも元に戻す
            va_end(ap);
            return 1;              
        }

        if (tc == ptnTc || ptnTc == TcExpr) {
            // 既にあるトークンと一致した

        } else if (ptnTc == TcType && (TcInt <= tc && tc <= TcFloat)) {
            // 変数の型
            if (tc == TcInt) {
                tVpc[vp++] = TyInt;
            } else if (tc == TcFloat) {
                tVpc[vp++] = TyFloat;
            }

        } else if (ptnTc == TcIdentifier && tc > TcEnd) {
            // 変数名, 識別子の時の処理
            tVpc[vp++] = tc;

        } else if (ptnTc == TcConst && tc > TcEnd) {
            // 定数の時の処理
            tVpc[vp++] = tc;

        } else if (ptnTc == TcExpr) {
            // 式の時の処理
            (*pc) = ppc;  // 式と思われるときはpcを元に戻して返す
            vp    = 0;  // 違うときはvpも元に戻す
            va_end(ap);
            return 1;                

        } else {
            (*pc) = ppc;  // もし一致しないときはpcをptnCmp()呼び出し前に戻す
            vp = 0;
            va_end(ap);
            return 0;  // 一致しなかったので0を返す
        }
        ptnTc = va_arg(ap, int32_t);  // 次のトークンパータンを参照
        (*pc)++;
    }

    if (nest != 0) {
        vp = 0;
        va_end(ap);
        return 0;  // ネストが正常でないので不一致
    }

    vp = 0;
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
void putIc(var_t **ic, int32_t *icp, int32_t op, var_t *v1, var_t *v2,
           var_t *v3, var_t *v4) {
    // printf("icp : %3d, Opcode : %3d, v1 : %d, v2 : %d, v3 : %d, v4 : %d\n", *icp, (int64_t)op, v1, v2, v3, v4);
    ic[(*icp)++] = (var_t *)((int64_t)op);
    ic[(*icp)++] = v1;
    ic[(*icp)++] = v2;
    ic[(*icp)++] = v3;
    ic[(*icp)++] = v4;
}

/* token列を内部コードに変換する */
void compile_sub(tokenBuf_t *tcBuf, var_t *var, var_t **ic, int32_t *icp, int32_t start, int32_t end) {
    // プログラムカウンタ
    int32_t pc = start;

    int32_t ppc = 0;  // ptnCmp()前のpcの値を保存しておく
    while (pc < end) {
        // printf("pc : %d, tc : %d\n", pc, tcBuf->tc[pc]);
        ppc = pc;

        if (ptnCmp(tcBuf, &pc, TcType, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            printf("<type> <identifier> = <const>;\n");
            // printf("tVpc[0] : %d, tVpc[1] : %d, tVpc[2] : %d\n", tVpc[0],
            // tVpc[1], tVpc[2]);
            putIc(ic, icp, OpDef, (var_t *)((int64_t)tVpc[0]), &var[tVpc[1]],
                  &var[tVpc[2]], 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            printf("<identifier> = <const>;\n");
            putIc(ic, icp, OpCpyS, &var[tVpc[0]], &var[tVpc[1]], 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcExpr)) {
            printf("<identifier> = <expr>;\n");
            pc += 2;  // 式の先頭までpcを進める
            expr(tcBuf, icp, &pc, var, ic, 0);
            putIc(ic, icp, OpCpyP, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcPlPlus, TcSemi)) {
            printf("<identifier>++;\n");
            putIc(ic, icp, OpAdd1, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcMiMinus, TcSemi)) {
            printf("<identifier>--;\n");
            putIc(ic, icp, OpSub1, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcPrint, TcIdentifier, TcSemi)) {
            printf("<print> <identifier>;\n");
            putIc(ic, icp, OpPrint, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIf, TcBrOpn, TcStop)) {
            printf("<if> (<expr>) {};\n");
            ifControl(tcBuf, icp, &pc, var, ic);
            
        } else if (ptnCmp(tcBuf, &pc, TcExpr)) {
            // TODO: エラー処理をちゃんとする
            printf("<expr>;\n");
            expr(tcBuf, icp, &pc, var, ic, 0);

        } else {
            goto err;
        }
    }
    return;

// エラー表示
err:
    fprintf(stderr, "compile error\n");
    exit(1);
}


/* 文字列sを内部コード列にコンパイルする関数 */
int32_t compile(str_t s, tokenBuf_t *tcBuf, var_t *var, var_t **ic) {
    int32_t end;    // コードの終わり
    end = lexer(s, tcBuf, var);

    // デバッグ用, tcの表示
    printf("tc : ");
    for (int i = 0; i < end; i++) {
        printf("%d ", tcBuf->tc[i]);
    }
    printf("\n");

    int32_t icp = 0;  // icをどこまで書き込んだか
    compile_sub(tcBuf, var, ic, &icp, 0, end);

    return 0;
}