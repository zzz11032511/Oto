#include <stdio.h>
#include <stdint.h>
#include "istack.h"

int32_t push(struct iStack *stack, int32_t value)
{
    stack->q[stack->sp++] = value;
    return value;
}

int32_t pop(struct iStack *stack)
{
    stack->sp--;
    return stack->q[stack->sp];
}

int32_t peek(struct iStack *stack)
{
    int32_t i = stack->sp - 1;
    return stack->q[i];
}