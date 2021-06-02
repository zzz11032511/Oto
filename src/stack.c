#include <stdio.h>
#include "stack.h"

int push(struct iStack *stack, int value)
{
    stack->q[stack->sp++] = value;
    return value;
}

int pop(struct iStack *stack)
{
    stack->sp--;
    return stack->q[stack->sp];
}

int peek(struct iStack *stack)
{
    int i = stack->sp - 1;
    return stack->q[i];
}