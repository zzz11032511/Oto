#include "iStack.h"

#include <stdint.h>
#include <stdio.h>

int32_t iPush(struct iStack *stack, int32_t value) {
    stack->q[stack->sp++] = value;
    return value;
}

int32_t iPop(struct iStack *stack) {
    stack->sp--;
    return stack->q[stack->sp];
}

int32_t iPeek(struct iStack *stack) {
    int32_t i = stack->sp - 1;
    return stack->q[i];
}