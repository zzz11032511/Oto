#pragma once

#include <stdint.h>
#include "token.h"
#include "../utils/util.h"
#include "../variable/variable.h"

/**
 *  プログラムをトークンコード列に変換する
 *  トークン列の長さを返す
 */
uint32_t lexer(str_t s, uint32_t fsize, tokenbuf_t *tcbuf, var_t *var_list);
