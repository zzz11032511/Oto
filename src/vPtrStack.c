#include <stdio.h>
#include "vPtrStack.h"

var_t *push(struct vPtrStack *stack, var_t *var)
{
    stack->q[stack->sp++] = var;
    return var;
}

var_t *pop(struct vPtrStack *stack)
{
    stack->sp--;
    return stack->q[stack->sp];
}

var_t *peek(struct vPtrStack *stack)
{
    return stack->q[stack->sp - 1];
}