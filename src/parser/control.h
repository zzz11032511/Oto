#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

/* "begin", '['の位置を渡すと、それに対応した"end", ']'の位置を返す */
uint32_t search_block_end(tokenbuf_t *tcbuf, uint32_t pc);

void loop_control(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp);

void if_control(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp);