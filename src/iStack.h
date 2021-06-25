#pragma once

#define MAX_STACK_SIZE 1000

struct iStack {
    int32_t q[MAX_STACK_SIZE];
    int32_t sp;
};

int32_t iPush(struct iStack *stack, int32_t value);
int32_t iPop(struct iStack *stack);
int32_t iPeek(struct iStack *stack);