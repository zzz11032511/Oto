#pragma once

#include <stdint.h>
#include "util.h"
#include "token.h"
#include "variable.h"

int32_t expr(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic, int32_t end);