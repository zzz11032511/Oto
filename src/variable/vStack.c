#include "vStack.h"
#include "stdarg.h"

#include <stdio.h>
#include "../errorHandle.h"
#include "../utils/util.h"

var_t push(struct vStack *stack, var_t var) {
    if (stack->sp >= VSTACK_SIZE) {
        callException(STACK_OVERFLOW_ERROR);
    } 
    stack->q[stack->sp++] = var;
    return var;
}

var_t constPush(struct vStack *stack, int64_t type, int64_t value) {
    if (stack->sp >= VSTACK_SIZE) {
        callException(STACK_OVERFLOW_ERROR);
    } 

    var_t var;
    var.type = type;
    var.value.iVal = value;

    stack->q[stack->sp++] = var;
    
    return var;
}

var_t pop(struct vStack *stack) {
    if (stack->sp <= 0) {
        callException(STACK_OVERFLOW_ERROR);
    }
    stack->sp--;
    return stack->q[stack->sp];
}

var_t peek(struct vStack *stack) {
    return stack->q[stack->sp - 1]; 
}