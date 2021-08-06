#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "control.h"
#include "expr.h"
#include "../error/error.h"
#include "../debug/debug.h"
#include "../utils/util.h"
#include "../vm/opcode.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../lexer/tokencode.h"
#include "../variable/variable.h"

/* トークンの識別子 */ 
#define TcExpr         -1    // 式
#define TcIdentifier   -2    // 識別子(変数やラベル)
#define TcOperator     -3    // 演算子
#define TcStop         -99   // 都合により構文評価を止めたいとき

uint32_t tmpvars[5];  // put_ic()で指定するTcを入れる
int32_t  vp = 0;   // tmpvarsへのポインタ

/* 引数に渡されたトークンのパターンと実際のコードが一致しているかを調べる */
bool_t ptn_cmp(tokenbuf_t *tcbuf, uint32_t *pc, int32_t pattern, ...) {
    va_list ap;
    va_start(ap, pattern);

    uint32_t ppc   = *pc;      // 最初のpcを保存しておく
    int32_t ptn_tc = pattern;

    while (1) {
        int32_t tc = tcbuf->tc_list[*pc];

        if (ptn_tc == TcLF && tc == TcLF) {
            (*pc)++;
            break;
        } 
        if (ptn_tc == TcStop) {
            (*pc) = ppc;       
            vp    = 0;
            va_end(ap);
            return true;              
        }

        if (tc == ptn_tc) {
            // 既にあるトークンと一致した

        } else if (ptn_tc == TcIdentifier && tc >= TcBegin) {
            // 演算子でないか(予約語も含む)
            tmpvars[vp++] = tc;

        } else if (ptn_tc == TcExpr) {
            (*pc) = ppc;
            vp    = 0;
            va_end(ap);
            return true;                

        } else {
            (*pc) = ppc;
            vp = 0;
            va_end(ap);
            return false;
        }
        ptn_tc = va_arg(ap, int32_t);
        (*pc)++;
    }

    vp = 0;
    va_end(ap);

    return true;
}

void put_ic(var_t **ic, uint32_t *icp, uint32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4) {
    ic[(*icp)++] = (var_t *)((uint64_t)op);
    ic[(*icp)++] = v1;
    ic[(*icp)++] = v2;
    ic[(*icp)++] = v3;
    ic[(*icp)++] = v4;
}

void compile_sub(tokenbuf_t *tcbuf, var_t *var_list, var_t **ic, uint32_t *icp, uint32_t start, uint32_t end) {
    // プログラムカウンタ
    uint32_t pc = start;

    while (pc < end) {
        if (ptn_cmp(tcbuf, &pc, TcImport, TcSqBrOpn, TcIdentifier, TcSqBrCls, TcLF)) {

        } else if (tcbuf->tc_list[pc] == TcLF) {
            pc++;

        } else if (ptn_cmp(tcbuf, &pc, TcDefine, TcIdentifier, TcColon, TcIdentifier, TcLF)) {
            uint32_t type = var_list[tmpvars[1]].type;
            if (is_rsvword_tc(tcbuf, tmpvars[0])) {
                call_error(NAME_ERROR);
            } else if (type != TyConstF) {
                call_error(ASSIGN_TO_LITERAL_ERROR);
            }
            
            var_list[tmpvars[0]].type = TyConstF;
            var_list[tmpvars[0]].value.fVal = var_list[tmpvars[1]].value.fVal;
        
        } else if (ptn_cmp(tcbuf, &pc, TcIdentifier, TcEqu, TcIdentifier, TcLF)) {
            // <identifier> = <identifier>;
            if (var_list[tmpvars[0]].type == TyConstF) {
                call_error(ASSIGN_TO_LITERAL_ERROR);
            } else if (is_rsvword_tc(tcbuf, tmpvars[0]) && is_rsvword_tc(tcbuf, tmpvars[1])) {
                call_error(NAME_ERROR);
            }
            put_ic(ic, icp, OpCpyD, &var_list[tmpvars[0]], &var_list[tmpvars[1]], 0, 0);

        } else if (ptn_cmp(tcbuf, &pc, TcIdentifier, TcEqu, TcExpr)) {
            // <identifier> = <expr>;
            if (var_list[tmpvars[0]].type == TyConstF) {
                call_error(ASSIGN_TO_LITERAL_ERROR);
            } else if (is_rsvword_tc(tcbuf, tmpvars[0])) {
                call_error(NAME_ERROR);
            }
            pc += 2;  // 式の先頭までpcを進める
            expr(tcbuf, &pc, 0, var_list, ic, icp);
            put_ic(ic, icp, OpCpyP, &var_list[tmpvars[0]], 0, 0, 0);

        } else if (ptn_cmp(tcbuf, &pc, TcPrint, TcIdentifier, TcLF)) {
            // print <identifier>
            put_ic(ic, icp, OpPrint, &var_list[tmpvars[0]], 0, 0, 0);

        } else if (ptn_cmp(tcbuf, &pc, TcLoop, TcStop)) {
            loop_control(tcbuf, &pc, var_list, ic, icp);

        } else if (ptn_cmp(tcbuf, &pc, TcIf, TcStop)) {
            if_control(tcbuf, &pc, var_list, ic, icp);

        } else if (ptn_cmp(tcbuf, &pc, TcExit)) {
            // exit
            put_ic(ic, icp, OpExit, 0, 0, 0, 0);

        } else {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }
    return;
}

void compile(str_t s, tokenbuf_t *tcbuf, var_t *var_list, var_t **ic) {
    tc_init(tcbuf, var_list);

    uint32_t end = lexer(s, tcbuf, var_list);

#ifdef DEBUG
    print_tokenlist(tcbuf);
    print_converted_source(tcbuf, end);
#endif

    uint32_t icp = 0;  // icをどこまで書き込んだか
    compile_sub(tcbuf, var_list, ic, &icp, 0, end);

    put_ic(ic, &icp, OpExit, 0, 0, 0, 0);
}