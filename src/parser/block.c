#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "compile.h"
#include "../error/error.h"
#include "../utils/util.h"
#include "../vm/opcode.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../lexer/tokencode.h"
#include "../variable/variable.h"

uint32_t seek_block_end(tokenbuf_t *tcbuf, uint32_t pc, uint32_t end_tc) {
    int32_t nest = 0;

    uint32_t tc = tcbuf->tc_list[pc];
    while (1) {
        if (tc == TcBegin || tc == TcSqBrOpn) {
            nest++;
        } else if (tc == TcEnd || tc == TcSqBrCls) {
            nest--;
        } else if (tc == TcExit) {
            call_error(INVALID_SYNTAX_ERROR);
        }

        if (tc == end_tc && nest == 0) {
            break;
        } else if (nest <= -1) {
            call_error(INVALID_SYNTAX_ERROR);
        }

        pc++;
        tc = tcbuf->tc_list[pc];
    }

    return pc;
}

void compile_args(tokenbuf_t *tcbuf, uint32_t pc, uint32_t end, var_t* var_list, var_t **ic, uint32_t *icp, uint32_t num_of_params) {
    if (num_of_params == 0) {
        return;
    }

    uint32_t params = 1;
    uint32_t tc = tcbuf->tc_list[pc];
    while (1) {
        // printf("pc : %d, tc : %d\n", pc, tc);
        if (params > num_of_params) {
            call_error(SYNTAX_ERROR);
        }

        if (IS_VALID_NAME(tc)) {
            uint32_t len = 1;
            while (tcbuf->tc_list[pc + len] != TcComma && tcbuf->tc_list[pc + len] != TcLF && (pc + len) <= end) {
                len++;
            }
            // printf("len : %d\n", len);
            if (len == 1) {
                put_ic(ic, icp, OpPush, &var_list[tc], 0, 0, 0); 
            } else {
                // 引数に式か関数が指定されている
                uint32_t ppc = pc;
                expr(tcbuf, &ppc, (pc + len - 1), var_list, ic, icp);
            }
            pc += len;

        } else if (tc == TcComma) {
            // 引数が省略されているとき
            put_ic(ic, icp, OpPushC, 0, (var_t *)TyInitVal, 0, 0);

        } else if (params == num_of_params) {
            // 最後の引数が省略されているとき
            put_ic(ic, icp, OpPushC, 0, (var_t *)TyInitVal, 0, 0);
            pc++;
        } else {
            call_error(SYNTAX_ERROR);
        }

        if (pc >= end) {
            break;
        }

        tc = tcbuf->tc_list[pc];
        if (tc == TcComma) {
            pc++;
            tc = tcbuf->tc_list[pc];
        } else {
            call_error(SYNTAX_ERROR);
        }

        params++;
    }
}

void compile_statement(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp) {
    uint32_t ppc = *pc;
    uint32_t statement = tcbuf->tc_list[ppc];
    ppc++;

    // 引数部分の終わりを取得する
    uint32_t end = seek_block_end(tcbuf, ppc, TcLF);

    switch (statement) {
    case TcBeep:
        compile_args(tcbuf, ppc, end, var_list, ic, icp, 2);
        put_ic(ic, icp, OpBeep, 0, 0, 0, 0);
        break;
    case TcPrint:
        compile_args(tcbuf, ppc, end, var_list, ic, icp, 1);
        put_ic(ic, icp, OpPrint, 0, 0, 0, 0);
        break;
    case TcPlay:
        compile_args(tcbuf, ppc, end, var_list, ic, icp, 4);
        put_ic(ic, icp, OpPlay, 0, 0, 0, 0);
        break;
    }

    *pc = end;
}

void compile_filter(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp) {
    uint32_t ppc = *pc;
    uint32_t end = seek_block_end(tcbuf, ppc, TcLF);

    uint32_t new_sound_tc = end - 1;
    if (!IS_VALID_NAME(tcbuf->tc_list[new_sound_tc])) {
        call_error(INVALID_SYNTAX_ERROR);
    }

    // 新しいSoundを定義する
    var_t *old = &var_list[tcbuf->tc_list[ppc]];
    var_t *new = &var_list[tcbuf->tc_list[new_sound_tc]];
    put_ic(ic, icp, OpCpyS, new, old, 0, 0);

    end -= 2;  // filter後のSoundの手前(フィルター部分)
    ppc += 2;  // 先頭のフィルター

    uint32_t tc = tcbuf->tc_list[ppc];
    while (ppc < end) {
        if (IS_VALID_NAME(tc)) {
            // 引数の数の取得
            // NOTE: 予め定義されたFilterしか、引数の数はvalueに入っていない

            // 引数のPUSH
            int32_t arg_end = seek_block_end(tcbuf, ppc + 1, TcSqBrCls);
            compile_args(tcbuf, ppc + 2, arg_end, var_list, ic, icp, 99);

            put_ic(ic, icp, OpFilter, new, &var_list[tc], 0, 0);
            ppc = arg_end + 1;
        } else {
            call_error(SYNTAX_ERROR);
        }

        tc = tcbuf->tc_list[ppc];

        // printf("ppc : %d, end : %d\n", ppc, end);
        if (ppc >= end) {
            break;
        } else if (tc == TcRArrow) {
            ppc++;
            tc = tcbuf->tc_list[ppc];
        } else {
            call_error(SYNTAX_ERROR);
        }
    }
    
    ppc = end + 2;
    *pc = ppc;
}