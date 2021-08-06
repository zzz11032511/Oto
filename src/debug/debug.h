#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

void print_opcodes(tokenbuf_t *tcbuf, var_t **ic);

void print_converted_source(tokenbuf_t *tcbuf, uint32_t len);

void print_tokenlist(tokenbuf_t *tcbuf);

/* 数式を逆ポーランド記法にしたものを出力する */
void print_rpn_tc(tokenbuf_t *tcbuf, uint32_t *rpn_tc_list, uint32_t len);