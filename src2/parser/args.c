#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "parser_func.h"
#include "../util/util.h"
#include "../error/error.h"
#include "../token/token.h"
#include "../ic/ic.h"

uint32_t seek_arg_end(uint32_t start) {
    uint32_t cur = start;

    uint32_t nest = 0;
    uint32_t end_of_src = get_conv_source_size();

    while (1) {
        tokencode_t tc = get_tc(cur++);
        if (tc == TcSqBrCls || tc == TcEnd) {
            nest--;
            if (nest < 0) {
                call_error(INVALID_SYNTAX_ERROR);
            }

        } else if (tc == TcSqBrOpn || tc == TcBegin || tc == TcIf) {
            nest++;

        }
        
        if (cur == end_of_src) {
            return cur;
            
        } else if (nest == 0 && (tc == TcComma || tc == TcLF)) {
            return cur - 1;
        }
    }
}

void parser_args(uint32_t *icp, uint32_t start, uint32_t end, uint32_t max_of_params) {
    if (max_of_params == 0) {
        return;
    }

    uint32_t params = 1;
    uint32_t cur = start;
    while (1) {
        tokencode_t tc = get_tc(cur);
        if (params > max_of_params) {
            call_error(SYNTAX_ERROR);
        }

        uint32_t arg_end = seek_arg_end(cur);
        uint32_t arg_len = arg_end - cur;
        printf("end : %d, cur : %d, arg_end : %d, arg_len : %d\n", end, cur, arg_end, arg_len);
        if (arg_len == 1) {
            put_ic(icp, OpPush, VAR_P(tc), 0, 0, 0);
        
        } else if (arg_len == 0) {
            put_ic(icp, OpPushC, (var_t *)TyInitVal, 0, 0, 0);

        } else {
            uint32_t cur2 = cur;
            expr(&cur2, arg_end, icp);
        }

        params++;
        
        cur = arg_end + 1;

        if (cur >= end) {
            break;
        }
    }
}