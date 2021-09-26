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

void parser_instruction(uint32_t *cur, uint32_t *icp) {
    uint32_t cur2 = *cur;

    uint32_t args_start = cur2 + 1;
    uint32_t args_end   = seek_args_block_end(args_start, TcLF);

    // printf("args_start : %d, args_end : %d\n", args_start, args_end);

    tokencode_t tc = get_tc(cur2);
    switch (tc) {
    case TcPrint:
        parser_args(icp, args_start, args_end, 1);
        put_ic(icp, OpPrint, 0, 0, 0, 0);
        break;

    case TcPlay:
        parser_args(icp, args_start, args_end, 4);
        put_ic(icp, OpPlay, 0, 0, 0, 0);
        break;
    
    case TcBeep:
        parser_args(icp, args_start, args_end, 2);
        put_ic(icp, OpBeep, 0, 0, 0, 0);
        break;
    
    default:
        call_error(SYNTAX_ERROR);
    }

    *cur = args_end + 1;
}