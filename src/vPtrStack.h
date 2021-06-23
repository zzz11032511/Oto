#pragma once

#include <stdint.h>
#include "variable.h"

#define MAX_STACK_SIZE 1000

struct vPtrStack {
    var_t *q[MAX_STACK_SIZE];
    int32_t sp;
};

var_t *push(struct vPtrStack *stack, var_t *var);
var_t *pop(struct vPtrStack *stack);
var_t *peek(struct vPtrStack *stack);

#endif 