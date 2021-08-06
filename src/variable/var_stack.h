#pragma once

#include <stdint.h>
#include "variable.h"

#define VSTACK_SIZE 1000

struct var_stack {
    var_t q[VSTACK_SIZE];
    int32_t sp;
};

var_t vpush(struct var_stack *stack, var_t var);
var_t const_vpush(struct var_stack *stack, uint64_t type, int64_t value);
var_t vpop(struct var_stack *stack);
var_t vpeek(struct var_stack *stack);