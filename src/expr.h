#pragma once

#include <stdint.h>
#include "util.h"
#include "variable.h"

int32_t expr(tokenBuf_t *tcBuf, int32_t *pc, var_t **var, int32_t **ic);