#ifndef INCLUDED_VPTRSTACK
#define INCLUDED_VPTRSTACK 1

#include "variable.h"

#define MAX_STACK_SIZE 1000

struct vPtrStack {
    var_t *q[MAX_STACK_SIZE];
    int sp;
};

var_t *push(struct vPtrStack *stack, var_t *var);
var_t *pop(struct vPtrStack *stack);
var_t *peek(struct vPtrStack *stack);

#endif 