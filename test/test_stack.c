#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "../src/iStack.h"

// iStackのテスト
int main(void)
{
    int32_t testarray[] = {1, 2, 3, 4, 5, 6, 7};
    int32_t len = sizeof(testarray);

    struct iStack stack;

    stack.sp = 0;

    int32_t i = 0;
    for (i = 0; i < len; i++) {
        assert(push(&stack, testarray[i]) == testarray[i]);
    }
    i--;

    assert(pop(&stack)  == testarray[i--]);
    assert(pop(&stack)  == testarray[i--]);
    assert(peek(&stack) == testarray[i]);
    assert(pop(&stack)  == testarray[i--]);
    assert(pop(&stack)  == testarray[i--]);
    assert(peek(&stack) == testarray[i]);
    assert(pop(&stack)  == testarray[i--]);

    printf("success\n");
    
    return 0;
}