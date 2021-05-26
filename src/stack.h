#ifndef INCLUDED_STACK
#define INCLUDED_STACK 1

#define MAX_STACK_SIZE 1000

struct iStack {
    int q[MAX_STACK_SIZE];
    int sp;
};

void push(struct iStack *stack, int value);
int pop(struct iStack *stack);
int peek(struct iStack *stack);

#endif 