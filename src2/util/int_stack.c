#include "int_stack.h"

#include <stdio.h>
#include <stdint.h>
#include "../error/error.h"

int32_t ipush(struct int_stack *stack, int32_t value) {
    if (stack->sp >= ISTACK_SIZE) {
        call_error(STACK_OVERFLOW_ERROR);
    }
    stack->q[stack->sp++] = value;
    return value;
}

int32_t ipop(struct int_stack *stack) {
    if (stack->sp <= 0) {
        call_error(STACK_OVERFLOW_ERROR);
    }
    stack->sp--;
    return stack->q[stack->sp];
}

int32_t ipeek(struct int_stack *stack) {
    int32_t i = stack->sp - 1;
    return stack->q[i];
}
