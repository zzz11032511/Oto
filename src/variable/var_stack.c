#include "var_stack.h"

#include <stdio.h>
#include "../error/error.h"
#include "../utils/util.h"

var_t vpush(struct var_stack *stack, var_t var) {
    if (stack->sp >= VSTACK_SIZE) {
        call_exception(STACK_OVERFLOW_ERROR);
    } 
    stack->q[stack->sp++] = var;
    return var;
}

var_t const_vpush(struct var_stack *stack, uint64_t type, int64_t value) {
    if (stack->sp >= VSTACK_SIZE) {
        call_exception(STACK_OVERFLOW_ERROR);
    } 

    var_t var;
    var.type = type;
    var.value.iVal = value;

    stack->q[stack->sp++] = var;
    
    return var;
}

var_t vpop(struct var_stack *stack) {
    if (stack->sp <= 0) {
        call_exception(STACK_OVERFLOW_ERROR);
    }
    stack->sp--;
    return stack->q[stack->sp];
}

var_t vpeek(struct var_stack *stack) {
    return stack->q[stack->sp - 1]; 
}