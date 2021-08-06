#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compile.h"
#include "expr.h"
#include "../error/error.h"
#include "../utils/util.h"
#include "../vm/opcode.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../lexer/tokencode.h"
#include "../variable/variable.h"

uint32_t get_argtcs(tokenbuf_t *tcbuf, uint32_t start, uint32_t end, uint32_t *argv, uint32_t arg_len) {
    uint32_t ppc = start + 1;
    uint32_t arg_cnt = 0;

    while (ppc < end) {
        uint32_t tc = tcbuf->tc_list[ppc];
        
        uint32_t cnt = 0;
        while((ppc + cnt) < end && tcbuf->tc_list[ppc + cnt] != TcComma) {
            cnt++;
        }

        if (cnt >= 2) {
            argv[arg_cnt++] = -1;
        } else {
            argv[arg_cnt++] = tc;
        }
        ppc += cnt;
    }

    if (arg_len < arg_cnt) {
        call_error(SYNTAX_ERROR);
    }

    return arg_cnt;
}

/* "then"の位置を渡すと、それに対応した"elsif", "else", "end"の位置を返す */
uint32_t search_ifblock_end(tokenbuf_t *tcbuf, uint32_t pc) {
    uint32_t nest = 1;

    while (1) {
        uint32_t tc = tcbuf->tc_list[pc++];
        if (tc == TcIf || tc == TcBegin) {
            nest++;
            continue;
        } else if (tc == TcElsif && nest == 1) {
            break;
        } else if (tc == TcElse && nest == 1) {
            break;
        } else if (tc == TcEnd) {
            nest--;
            if (nest != 0) continue;
            else break;
        } else if (tc == TcExit) {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }

    return pc - 1;
}

uint32_t search_elseblock_end(tokenbuf_t *tcbuf, uint32_t pc) {
    uint32_t nest = 1;

    while (1) {
        uint32_t tc = tcbuf->tc_list[pc++];
        if (tc == TcIf || tc == TcBegin) {
            nest++;
            continue;
        } else if (tc == TcEnd) {
            nest--;
            if (nest != 0) continue;
            else break;
        } else if (tc == TcExit) {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }

    return pc - 1;
}

/* "begin", '['の位置を渡すと、それに対応した"end", ']'の位置を返す */
uint32_t search_block_end(tokenbuf_t *tcbuf, uint32_t pc) {
    uint32_t nest = 0;

    if (tcbuf->tc_list[pc] == TcThen) {
        return search_ifblock_end(tcbuf, pc);
    } else if (tcbuf->tc_list[pc] == TcElse) {
        return search_elseblock_end(tcbuf, pc);
    }

    while (1) {
        uint32_t tc = tcbuf->tc_list[pc++];
        if (tc == TcBegin || tc == TcSqBrOpn || tc == TcIf) {
            nest++;
            continue;
        } else if (tc == TcEnd || tc == TcSqBrCls) {
            nest--;
            if (nest != 0) continue;
            else break;
        } else if (tc == TcExit) {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }

    return pc - 1;
}

/* "begin-end"で囲まれたブロック内のコードをコンパイルし、ブロックを抜けた場所のpcを返す */
int32_t block_compile(tokenbuf_t *tcbuf,  uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp) {
    uint32_t start = *pc + 1;
    uint32_t end   = search_block_end(tcbuf, *pc) - 1;

#ifdef DEBUG
    printf("block : start [%d], end [%d]\n", start, end);
#endif

    compile_sub(tcbuf, var_list, ic, icp, start, end);
    return end + 2;    // ブロックの外に持っていくため2足す
}

void loop_control(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp) {
    uint32_t ppc = *pc + 1;

    uint32_t argv[] = {0, 0, 0};
    uint32_t start = ppc;
    uint32_t end = search_block_end(tcbuf, start);

    get_argtcs(tcbuf, start, end, argv, GET_ARRAY_LENGTH(argv));
    ppc = end + 1;

    uint32_t jmp_icp = *icp;
    put_ic(ic, icp, OpLoop, 0, 0, 0, 0);

    ppc = block_compile(tcbuf, &ppc, var_list, ic, icp);

    put_ic(ic, icp, OpJmp, (var_t *)((uint64_t)jmp_icp), 0, 0, 0);
    put_ic(ic, &jmp_icp, OpLoop, (var_t *)((uint64_t)*icp), &var_list[argv[0]], 0, 0);

    *pc = ppc;

    return; 
}

void if_control(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp) {
    uint32_t ppc = *pc + 1;

    uint32_t not_flag = 0;
    if (tcbuf->tc_list[ppc] == TcNot) {
        not_flag = 1;
        ppc++;
    }

    // 条件式部分
    uint32_t start = ppc;
    uint32_t end = search_block_end(tcbuf, start);
    start++;

    expr(tcbuf, &start, end - 1, var_list, ic, icp);
    ppc = end + 1;

    // 処理部分
    uint32_t jmp_icp1 = *icp;
    put_ic(ic, icp, OpJz, 0, 0, 0, 0);

    ppc = block_compile(tcbuf, &ppc, var_list, ic, icp);

    if (tcbuf->tc_list[ppc - 1] == TcElsif) {
        ppc--;
        uint32_t jmp_icp2 = *icp;
        put_ic(ic, icp, OpJmp, 0, 0, 0, 0);

        if (not_flag) {
            put_ic(ic, &jmp_icp1, OpJnz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        } else {
            put_ic(ic, &jmp_icp1, OpJz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        }

        if_control(tcbuf, &ppc, var_list, ic, icp);
        put_ic(ic, &jmp_icp2, OpJmp, (var_t *)((uint64_t)*icp), 0, 0, 0);

    } else if (tcbuf->tc_list[ppc - 1] == TcElse) {
        ppc--;
        uint32_t jmp_icp3 = *icp;
        put_ic(ic, icp, OpJmp, 0, 0, 0, 0);

        if (not_flag) {
            put_ic(ic, &jmp_icp1, OpJnz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        } else {
            put_ic(ic, &jmp_icp1, OpJz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        }

        ppc = block_compile(tcbuf, &ppc, var_list, ic, icp);

        put_ic(ic, &jmp_icp3, OpJmp, (var_t *)((uint64_t)*icp), 0, 0, 0);

    } else {
        if (not_flag) {
            put_ic(ic, &jmp_icp1, OpJnz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        } else {
            put_ic(ic, &jmp_icp1, OpJz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        }

    }

    *pc = ppc;

    return; 
}