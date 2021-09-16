#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "parser_func.h"
#include "../opcode.h"
#include "../util/util.h"
#include "../util/int_stack.h"
#include "../error/error.h"
#include "../debug/debug.h"
#include "../token/token.h"
#include "../variable/type.h"
#include "../ic/ic.h"

uint32_t seek_block_if_end(uint32_t start) {
    uint32_t nest = 1;
    uint32_t end_of_src = get_conv_source_size();

    uint32_t end = start;
    while (1) {
        tokencode_t tc = get_tc(end++);
        if (tc == TcElsif && nest == 1) {
            break;
        } else if (tc == TcElse && nest == 1) {
            break;
        } if (tc == TcEnd) {
            nest--;
            if (nest == 0) {
                break;
            } else if (nest < 0) {
                call_error(INVALID_SYNTAX_ERROR);
            }

        } else if (tc == TcBegin || tc == TcIf) {
            nest++;
        } else if (end > end_of_src) {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }

    return end - 1;
}

void parser_if(uint32_t *cur, uint32_t *icp) {
    uint32_t cur2 = *cur + 1;

    bool not_flag = false;
    if (get_tc(cur2) == TcNot) {
        not_flag = true;
        cur2++;
    }

    // 条件式部分
    uint32_t start = cur2;
    uint32_t end = seek_block_end(start);
    start++;

    expr(&start, end, icp);
    cur2 = end + 1;

    // 処理部分
    uint32_t jmp_icp1 = *icp;
    put_ic(icp, OpJz, 0, 0, 0, 0);

    start = cur2 + 1;  // Thenの位置 + 1
    end = seek_block_if_end(start);
    parser_sub(icp, start, end);

    // printf("start : %d, end : %d\n", start, end);
    
    cur2 = end;  // (End | Elsif | Else)の位置
    if (get_tc(cur2) == TcElsif) {
        uint32_t jmp_icp2 = *icp;
        put_ic(icp, OpJmp, 0, 0, 0, 0);

        if (not_flag) {
            put_ic(&jmp_icp1, OpJnz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        } else {
            put_ic(&jmp_icp1, OpJz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        }

        parser_if(&cur2, icp);
        put_ic(&jmp_icp2, OpJmp, (var_t *)((uint64_t)*icp), 0, 0, 0);

    } else if (get_tc(cur2) == TcElse) {
        uint32_t jmp_icp3 = *icp;
        put_ic(icp, OpJmp, 0, 0, 0, 0);

        if (not_flag) {
            put_ic(&jmp_icp1, OpJnz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        } else {
            put_ic(&jmp_icp1, OpJz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        }

        start = cur2 + 1;  // Elseの位置 + 1
        end = seek_block_if_end(start);

        parser_sub(icp, start, end - 1);
        put_ic(&jmp_icp3, OpJmp, (var_t *)((uint64_t)*icp), 0, 0, 0);

        cur2 = end + 1;

    } else {
        if (not_flag) {
            put_ic(&jmp_icp1, OpJnz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        } else {
            put_ic(&jmp_icp1, OpJz, (var_t *)((uint64_t)*icp), 0, 0, 0);
        }

        cur2 = end + 1;
    }

    *cur = cur2;
}

void parser_loop(uint32_t *cur, uint32_t *icp) {
    uint32_t cur2 = *cur + 1;

    uint32_t start = cur2;

    // 引数が1個の場合のみ
    uint32_t arg_tc = get_tc(start + 1);
    
    uint32_t end = seek_block_end(start);

    cur2 = end + 1;

    uint32_t jmp_icp = *icp;
    put_ic(icp, OpLoop, 0, 0, 0, 0);

    start = cur2;
    end = seek_block_end(start);
    
    // printf("start : %d, end : %d\n", start, end);

    parser_sub(icp, start + 1, end - 1);

    put_ic(icp, OpJmp, (var_t *)((uint64_t)jmp_icp), 0, 0, 0);
    put_ic(&jmp_icp, OpLoop, (var_t *)((uint64_t)*icp), VAR_P(arg_tc), 0, 0);

    *cur = end + 1;
}