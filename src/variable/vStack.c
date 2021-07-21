#include "vStack.h"

#include <stdio.h>
#include "../errorHandle.h"

var_t push(struct vStack *stack, var_t var) {
    if (stack->sp >= STACK_SIZE) {
        exception(STACK_OVERFLOW_ERROR);
    } 
    stack->q[stack->sp++] = var;
    return var;
}

var_t pop(struct vStack *stack) {
    if (stack->sp <= 0) {
        exception(STACK_OVERFLOW_ERROR);
    }
    stack->sp--;
    return stack->q[stack->sp];
}

var_t peek(struct vStack *stack) {
    return stack->q[stack->sp - 1]; 
}