#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "../error/error.h"
#include "../debug/debug.h"
#include "../utils/util.h"
#include "../vm/opcode.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../lexer/tokencode.h"
#include "../variable/variable.h"

/* トークンの識別子 */ 
#define PtnLabel        -1    // 変数かラベル
#define PtnExpr         -2    // 式
#define PtnImpr         -3    // 命令
#define PtnStop         -99   // 都合により構文評価を止めたいとき

#define TMPVARS_LENGTH 6
uint32_t tmpvars[TMPVARS_LENGTH];  // put_ic()で指定するTcを入れる
uint32_t vp = 1;   // tmpvarsへのポインタ(1が先頭)

void reset_tmpvars() {
    for (int32_t i = 0; i < TMPVARS_LENGTH; i++) {
        tmpvars[i] = 0;
    }
    vp = 1;
}

bool_t ptn_cmp(tokenbuf_t *tcbuf, uint32_t pc, uint32_t pattern, ...) {
    va_list ap;
    va_start(ap, pattern);

    uint32_t ptn_tc = pattern;

    reset_tmpvars();
    while (ptn_tc != PtnStop) {
        uint32_t tc = tcbuf->tc_list[pc];

        if (tc == ptn_tc) {
            // パターンが一致した場合
        } else if (ptn_tc == PtnImpr && IS_STATEMENT(tc)) {
            // 命令だった場合
        } else if (ptn_tc == PtnLabel && IS_NOT_SYMBOL(tc)) {
            tmpvars[vp++] = tc;
        }else if (ptn_tc == PtnExpr) {
            break;
        } else {
            va_end(ap);
            return false;
        }
        ptn_tc = va_arg(ap, uint32_t);
        pc++;
    }

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

void check_assignment_error(tokenbuf_t *tcbuf, var_t *var_list) {
    if (var_list[tmpvars[1]].type == TyConst) {
        call_error(ASSIGN_TO_LITERAL_ERROR);
    } else if (IS_RSVWORD(tmpvars[1]) || IS_RSVWORD(tmpvars[2]) 
               || IS_RSVWORD(tmpvars[3]) || IS_RSVWORD(tmpvars[4])) {
        call_error(NAME_ERROR);
    }
}

void compile_sub(tokenbuf_t *tcbuf, var_t *var_list, var_t **ic, uint32_t *icp, uint32_t start, uint32_t end) {
    uint32_t pc = start;

    while (pc < end) {
        if (ptn_cmp(tcbuf, pc, TcImport, TcSqBrOpn, PtnLabel, TcSqBrCls, TcLF, PtnStop)) {
            // import文は読み飛ばす
        } else if (tcbuf->tc_list[pc] == TcLF) {
            pc++;

        } else if (ptn_cmp(tcbuf, pc, TcDefine, PtnLabel, TcColon, PtnLabel, TcLF, PtnStop)) {
            /* define <label> : <const> */
            if (var_list[tmpvars[2]].type != TyConst) {
                call_error(DEFINE_ERROR);
            }
            check_assignment_error(tcbuf, var_list);
            var_list[tmpvars[1]].type = TyConst;
            var_list[tmpvars[1]].value.fVal = var_list[tmpvars[2]].value.fVal;
            pc += 5;

        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcEqu, PtnLabel, TcLF, PtnStop)) {
            /* <var> = <var> */
            check_assignment_error(tcbuf, var_list);
            put_ic(ic, icp, OpCpyD, &var_list[tmpvars[1]], &var_list[tmpvars[2]], 0, 0);
            pc += 4;

        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcEqu, PtnLabel, TcPlus, PtnLabel, TcLF, PtnStop)) {
            /* <var> = <var> % <var> */
            check_assignment_error(tcbuf, var_list);
            put_ic(ic, icp, OpAdd2, 
                   &var_list[tmpvars[1]], &var_list[tmpvars[2]], &var_list[tmpvars[3]], 0);
            pc += 6;
        
        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcEqu, PtnLabel, TcMinus, PtnLabel, TcLF, PtnStop)) {
            /* <var> = <var> % <var> */
            check_assignment_error(tcbuf, var_list);
            put_ic(ic, icp, OpSub2, 
                   &var_list[tmpvars[1]], &var_list[tmpvars[2]], &var_list[tmpvars[3]], 0);
            pc += 6;

        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcEqu, PtnLabel, TcAster, PtnLabel, TcLF, PtnStop)) {
            /* <var> = <var> % <var> */
            check_assignment_error(tcbuf, var_list);
            put_ic(ic, icp, OpMul2, 
                   &var_list[tmpvars[1]], &var_list[tmpvars[2]], &var_list[tmpvars[3]], 0);
            pc += 6;

        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcEqu, PtnLabel, TcSlash, PtnLabel, TcLF, PtnStop)) {
            /* <var> = <var> % <var> */
            check_assignment_error(tcbuf, var_list);
            put_ic(ic, icp, OpDiv2, 
                   &var_list[tmpvars[1]], &var_list[tmpvars[2]], &var_list[tmpvars[3]], 0);
            pc += 6;

        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcEqu, PtnLabel, TcPerce, PtnLabel, TcLF, PtnStop)) {
            /* <var> = <var> % <var> */
            check_assignment_error(tcbuf, var_list);
            put_ic(ic, icp, OpMod2, 
                   &var_list[tmpvars[1]], &var_list[tmpvars[2]], &var_list[tmpvars[3]], 0);
            pc += 6;

        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcEqu, PtnExpr)) {
            /* <var> = <expr> */
            check_assignment_error(tcbuf, var_list);
            pc += 2;  /* 式の先頭までpcを進める */
            expr(tcbuf, &pc, 0, var_list, ic, icp);
            put_ic(ic, icp, OpCpyP, &var_list[tmpvars[1]], 0, 0, 0);

        } else if (ptn_cmp(tcbuf, pc, PtnImpr, PtnStop)) {
            /* <imperative> <arg1>, <arg2>, ... */
            compile_statement(tcbuf, &pc, var_list, ic, icp);

        } else if (ptn_cmp(tcbuf, pc, TcLoop, PtnStop)) {
            /* loop */
            loop_control(tcbuf, &pc, var_list, ic, icp);

        } else if (ptn_cmp(tcbuf, pc, TcIf, PtnStop)) {
            /* if */
            if_control(tcbuf, &pc, var_list, ic, icp);

        } else if (ptn_cmp(tcbuf, pc, TcExit, TcLF, PtnStop)) {
            put_ic(ic, icp, OpExit, 0, 0, 0, 0);
            pc += 2;

        } else if (ptn_cmp(tcbuf, pc, PtnLabel, TcLF, PtnStop)) {
            pc += 2;

        } else {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }
}

void compile(str_t s, int32_t fsize, tokenbuf_t *tcbuf, var_t *var_list, var_t **ic) {
    init_token(tcbuf, var_list);

    uint32_t end = lexer(s, fsize, tcbuf, var_list);

#ifdef DEBUG
    print_tokenlist(tcbuf);
    print_converted_source(tcbuf, end);
#endif

    uint32_t icp = 0;  // icをどこまで書き込んだか
    compile_sub(tcbuf, var_list, ic, &icp, 0, end);

    put_ic(ic, &icp, OpExit, 0, 0, 0, 0);
}