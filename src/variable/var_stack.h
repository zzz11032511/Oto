#pragma once

#include <stdint.h>
#include "variable.h"

#define VSTACK_SIZE 1000

struct var_stack {
    Var q[VSTACK_SIZE];
    int32_t sp;
};

Var vpush(struct var_stack *stack, Var var);
Var const_vpush(struct var_stack *stack, uint64_t type, int64_t value);
Var vpop(struct var_stack *stack);
Var vpeek(struct var_stack *stack);
