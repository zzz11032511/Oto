#pragma once

#include "stdint.h"
#include "util.h"
#include "token.h"
#include "variable.h"

int32_t lexer(str_t s, tokenBuf_t *tcBuf, var_t **var);