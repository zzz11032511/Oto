#pragma once

#include <stdint.h>
#include "../../variable/variable.h"
#include "../../variable/var_stack.h"

var_t calculation(var_t v1, var_t v2, uint32_t opcode);
int32_t compare(var_t v1, var_t v2, uint32_t opcode);

void op_filter(struct var_stack *stack, var_t *sound, var_t *filter);