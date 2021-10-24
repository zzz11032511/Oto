#include "var_stack.h"

#include <stdio.h>
#include "../error/error.h"

Var vpush(struct var_stack *stack, Var var) {
    if (stack->sp >= VSTACK_SIZE) {
        call_error(STACK_OVERFLOW_ERROR);
    } 
    stack->q[stack->sp++] = var;
    return var;
}

Var const_vpush(struct var_stack *stack, uint64_t type, int64_t value) {
    if (stack->sp >= VSTACK_SIZE) {
        call_error(STACK_OVERFLOW_ERROR);
    } 

    Var var;
    var.type = type;
    var.value.iVal = value;

    stack->q[stack->sp++] = var;
    
    return var;
}

Var vpop(struct var_stack *stack) {
    if (stack->sp <= 0) {
        call_error(STACK_OVERFLOW_ERROR);
    }
    stack->sp--;
    return stack->q[stack->sp];
}

Var vpeek(struct var_stack *stack) {
    return stack->q[stack->sp - 1]; 
}
