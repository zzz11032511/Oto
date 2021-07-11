#include "vStack.h"

#include <stdio.h>

var_t push(struct vStack *stack, var_t var) {
    stack->q[stack->sp++] = var;
    return var;
}

var_t pop(struct vStack *stack) {
    stack->sp--;
    return stack->q[stack->sp];
}

var_t peek(struct vStack *stack) {
    return stack->q[stack->sp - 1]; 
}