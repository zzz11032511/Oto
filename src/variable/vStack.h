#pragma once

#include <stdint.h>
#include "variable.h"

#define VSTACK_SIZE 1000

struct vStack {
    var_t q[VSTACK_SIZE];
    int32_t sp;
};

var_t push(struct vStack *stack, var_t var);
var_t constPush(struct vStack *stack, int64_t type, int64_t value);
var_t pop(struct vStack *stack);
var_t peek(struct vStack *stack);