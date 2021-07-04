#pragma once

#include <stdint.h>
#include "token.h"
#include "variable.h"
#include "util.h"

void ifControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic, int32_t cmpStartPc, int32_t cmpEndPc);