#pragma once

#include <stdint.h>

#define ISTACK_SIZE 1000

struct int_stack {
    int32_t q[ISTACK_SIZE];
    int32_t sp;
};

int32_t ipush(struct int_stack *stack, int32_t value);
int32_t ipop(struct int_stack *stack);
int32_t ipeek(struct int_stack *stack);
