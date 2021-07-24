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

/* 引数に渡されたトークンのパターンと実際のコードが一致しているかを調べる関数 */
int32_t ptnCmp(tokenBuf_t *tcBuf, int32_t *pc, int32_t pattern, ...) {
    va_list ap;
    va_start(ap, pattern);

    int32_t ppc   = *pc;      // 最初のpcを保存しておく
    int32_t ptnTc = pattern;  // パターンから読み込んだトークン

    while (1) {
        int32_t tc = tcBuf->tc[*pc];

        if (ptnTc == TcSemi && tc == TcSemi) {
            // セミコロンなら終わり
            (*pc)++;
            break;
        } 
        if (ptnTc == TcStop) {
            (*pc) = ppc;       
            vp    = 0;
            va_end(ap);
            return 1;              
        }

        if (tc == ptnTc) {
            // 既にあるトークンと一致した

        } else if (ptnTc == TcIdentifier && tc > TcComma) {
            tVpc[vp++] = tc;

        } else if (ptnTc == TcExpr) {
            (*pc) = ppc;
            vp    = 0;
            va_end(ap);
            return 1;                

        } else {
            (*pc) = ppc;
            vp = 0;
            va_end(ap);
            return 0;
        }
        ptnTc = va_arg(ap, int32_t);
        (*pc)++;
    }

    vp = 0;
    va_end(ap);

    return 1;
}

void putIc(var_t **ic, int32_t *icp, int32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4) {
    ic[(*icp)++] = (var_t *)((int64_t)op);
    ic[(*icp)++] = v1;
    ic[(*icp)++] = v2;
    ic[(*icp)++] = v3;
    ic[(*icp)++] = v4;
}

void compile_sub(tokenBuf_t *tcBuf, var_t *var, var_t **ic, int32_t *icp, int32_t start, int32_t end) {
    // プログラムカウンタ
    int32_t pc = start;

    while (pc < end) {
        if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcIdentifier, TcSemi)) {
            // <identifier> = <identifier>;
            if (var[tVpc[0]].type == TyConstI || var[tVpc[0]].type == TyConstF) {
                callError(ASSIGN_TO_LITERAL_ERROR);
            } else if (isRsvWord(tcBuf, tVpc[0])) {
                callError(NAME_ERROR);
            }
            putIc(ic, icp, OpCpyS, &var[tVpc[0]], &var[tVpc[1]], 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIdentifier, TcEqu, TcExpr)) {
            // <identifier> = <expr>;
            if (var[tVpc[0]].type == TyConstI || var[tVpc[0]].type == TyConstF) {
                callError(ASSIGN_TO_LITERAL_ERROR);
            } else if (isRsvWord(tcBuf, tVpc[0])) {
                callError(NAME_ERROR);
            }
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
            // print <identifier>;
            putIc(ic, icp, OpPrint, &var[tVpc[0]], 0, 0, 0);

        } else if (ptnCmp(tcBuf, &pc, TcIf, TcBrOpn, TcStop)) {
            // if (<expr>);
            ifControl(tcBuf, icp, &pc, var, ic);
            
        } else if (ptnCmp(tcBuf, &pc, TcWhile, TcBrOpn, TcStop)) {
            // while (<expr>);
            whileControl(tcBuf, icp, &pc, var, ic);

        } else {
            printf("aaa\n");
            callError(INVALID_SYNTAX_ERROR);
        }
    }
    return;
}

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