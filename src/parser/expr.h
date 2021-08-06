#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

void expr(tokenbuf_t *tcbuf, uint32_t *pc, uint32_t end, var_t *var_list, var_t **ic, uint32_t *icp);