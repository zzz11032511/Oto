#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

/* 文字列sを内部コード列にコンパイルする関数 */
void compile(str_t s, uint32_t fsize, tokenbuf_t *tcbuf, var_t *var_list, var_t **ic);