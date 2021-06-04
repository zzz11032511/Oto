#include <stdio.h>
#include <assert.h>
#include "../src/stack.h"

// iStackのテスト
int main(void)
{
    int testarray[] = {1, 2, 3, 4, 5, 6, 7};
    int len = sizeof(testarray);

    struct iStack stack;

    stack.sp = 0;

    int i = 0;
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