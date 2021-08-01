#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

/* "begin", '['の位置を渡すと、それに対応した"end", ']'の位置を返す */
int32_t searchBlockEnd(tokenBuf_t *tcBuf, int32_t pc);

void loopControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic);

void ifControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic);