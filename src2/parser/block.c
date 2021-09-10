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

/**
 * startを始まりとしたブロック(BEGIN~END, (~), [~])の終端の場所を返す 
 * 返される場所は(END, ), ])の位置
 */
uint32_t seek_block_end(uint32_t start) {
    uint32_t nest = 0;
    uint32_t end = start;
    uint32_t end_of_src = get_conv_source_size();

    while (1) {
        tokencode_t tc = get_tc(end++);

        if (tc == TcSqBrCls || tc == TcEnd) {
            nest--;
            if (nest == 0) {
                break;
            } else if (nest < 0) {
                call_error(INVALID_SYNTAX_ERROR);
            }

        } else if (tc == TcSqBrOpn || tc == TcBegin || tc == TcIf) {
            nest++;
        } else if (end > end_of_src) {
            // 括弧が見つからない
            call_error(INVALID_SYNTAX_ERROR);
        }
    }
    
    return end - 1;
}
