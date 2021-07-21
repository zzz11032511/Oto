#include "compile.h"

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "control.h"
#include "expr.h"
#include "../errorHandle.h"
#include "../debug.h"
#include "../utils/util.h"
#include "../vm/ic.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

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
        // printf("ptnTc : %d, tc : %d, pc : %d\n", ptnTc, tc, *pc);

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

        if (tc == ptnTc) {
            // 既にあるトークンと一致した

        } else if (ptnTc == TcType && (TcInt <= tc && tc <= TcFloat)) {
            // 変数の型
            if (tc == TcInt) {
                tVpc[vp++] = TyInt;
            } else if (tc == TcFloat) {
                tVpc[vp++] = TyFloat;
            }

        } else if (ptnTc == TcIdentifier && tc >= TcEnd) {
            // 変数名, 識別子の時の処理
            tVpc[vp++] = tc;

        } else if (ptnTc == TcConst && tc >= TcEnd) {
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
void putIc(var_t **ic, int32_t *icp, int32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4) {
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

    while (pc < end) {
        if (ptnCmp(tcBuf, &pc, TcType, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            // <type> <identifier> = <const>;
            putIc(ic, icp, OpDef, (var_t *)((int64_t)tVpc[0]), &var[tVpc[1]],
                  &var[tVpc[2]], 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcConst, TcSemi)) {
            // <identifier> = <const>;
            putIc(ic, icp, OpCpyS, &var[tVpc[0]], &var[tVpc[1]], 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcExpr)) {
            // <identifier> = <expr>;
            pc += 2;  // 式の先頭までpcを進める
            expr(tcBuf, icp, &pc, var, ic, 0);
            putIc(ic, icp, OpCpyP, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcPlPlus, TcSemi)) {
            // <identifier>++;
            putIc(ic, icp, OpAdd1, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcMiMinus, TcSemi)) {
            // <identifier>--;
            putIc(ic, icp, OpSub1, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcPrint, TcIdentifier, TcSemi)) {
            // <print> <identifier>;
            putIc(ic, icp, OpPrint, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIf, TcBrOpn, TcStop)) {
            // <if> (<expr>) {};
            ifControl(tcBuf, icp, &pc, var, ic);
            
        } else if (ptnCmp(tcBuf, &pc, TcWhile, TcBrOpn, TcStop)) {
            // <while> (<expr>) {};
            whileControl(tcBuf, icp, &pc, var, ic);

        } else {
            callError(SYNTAX_ERROR);
        }
    }
    return;
}

/* 文字列sを内部コード列にコンパイルする関数 */
int32_t compile(str_t s, tokenBuf_t *tcBuf, var_t *var, var_t **ic) {
    tcInit(tcBuf, var);

    int32_t end = lexer(s, tcBuf, var);

#ifdef DEBUG
    printTokenCode(tcBuf, end);
#endif

    int32_t icp = 0;  // icをどこまで書き込んだか
    compile_sub(tcBuf, var, ic, &icp, 0, end);

    putIc(ic, &icp, OpEnd, 0, 0, 0, 0);

    return 0;
}