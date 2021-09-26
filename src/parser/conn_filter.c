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

void parser_connect_filters(uint32_t *cur, uint32_t *icp) {
    uint32_t cur2 = *cur;
    uint32_t end = seek_args_block_end(cur2, TcLF);
    
    tokencode_t s_sound_tc = get_tc(cur2);     // 接続元のSound
    tokencode_t e_sound_tc = get_tc(end - 1);  // 接続先のSound

    if (s_sound_tc != e_sound_tc) {
        put_ic(icp, OpCpyS, VAR_P(e_sound_tc), VAR_P(s_sound_tc), 0, 0);
    }

    if (!IS_VALID_NAME(e_sound_tc)) {
        call_error(SYNTAX_ERROR);
    }

    cur2 += 1;
    while (1) {
        if (get_tc(cur2) == TcRArrow) {
            cur2++;
            tokencode_t filter_tc = get_tc(cur2);

            if (cur2 == (end - 1) && filter_tc == e_sound_tc) {
                break;
            }

            cur2++;
            size_t len = seek_block_end(cur2) - cur2;

            if (len == 1) {
                call_error(SYNTAX_ERROR);
            } else {
                uint32_t start = cur2 + 1;
                uint32_t end = cur2 + len;
                parser_args_sub(icp, start, end);
                put_ic(icp, OpFilter, VAR_P(e_sound_tc), VAR_P(filter_tc), 0, 0);
            }
            cur2 += len + 1;

        } else if (get_tc(cur2) == TcLF) {
            break;
            
        } else {
            call_error(SYNTAX_ERROR);
        }    
    }

    *cur = cur2;
}