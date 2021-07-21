#pragma once

#include <stdint.h>
#include "variable.h"

#define STACK_SIZE 1000

struct vStack {
    var_t q[STACK_SIZE];
    int32_t sp;
};

var_t push(struct vStack *stack, var_t var);
var_t pop(struct vStack *stack);
var_t peek(struct vStack *stack);