#include <oto/oto_util.h>

#define DEFAULT_MAX_STACK_CAPACITY 100

Stack *new_stack() {
    Stack *stack = MYMALLOC1(Stack);
    if (IS_NULL(stack)) {
        return NULL;
    }

    stack->sp = 0;
    stack->datas = new_vector_ptr(DEFAULT_MAX_STACK_CAPACITY);
    if (IS_NULL(stack->datas)) {
        free(stack);
        return NULL;
    }

    return stack;
}

void free_stack(Stack *stack) {
    free_vector_ptr(stack->datas);
    free(stack);
}

void stack_push(Stack *stack, void *data) {
    vector_ptr_set(stack->datas, stack->sp, data);
    (stack->sp)++;
}

void stack_pushi(Stack *stack, int64_t data) {
    vector_ptr_set(stack->datas, stack->sp, (void *)data);
    (stack->sp)++;
}

void *stack_pop(Stack *stack) {
    if (stack->sp <= 0) {
        return NULL;
    }

    (stack->sp)--;
    return stack->datas->data[stack->sp];
}

int64_t stack_popi(Stack *stack) {
    if (stack->sp <= 0) {
        return 0;
    }

    (stack->sp)--;
    return (int64_t)stack->datas->data[stack->sp];
}

void *stack_peek(Stack *stack) {
    if (stack->sp <= 0) {
        return NULL;
    }

    int64_t i = stack->sp - 1;
    return stack->datas->data[i];
}

int64_t stack_peeki(Stack *stack) {
    if (stack->sp <= 0) {
        return 0;
    }

    int64_t i = stack->sp - 1;
    return (int64_t)stack->datas->data[i];
}
