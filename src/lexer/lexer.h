#pragma once

#include <stdint.h>
#include "token.h"
#include "../utils/util.h"
#include "../variable/variable.h"

/* プログラムをトークンコード列に変換する */
int32_t lexer(str_t s, tokenBuf_t *tcBuf, var_t *var);