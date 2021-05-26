#include <stdio.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"
#include "compile.h"
#include "stack.h"

void exec(int **ic, int *var)
{
    int **icp = ic;

    // 計算用のスタック
    struct iStack stack;
    stack.sp = 0;
    int t1;    // 演算用の一時変数
    int t2;   

    while (1) {
        // printf("opcode : %d\n", (int)icp[0]);
        switch ((int) icp[0]) {
        case OpCpy:
            *icp[1] = pop(&stack);
            continue;

        case OpAdd:
            t1 = pop(&stack);
            t2 = pop(&stack);
            printf("%d\n", t2 + t1);
            push(&stack, t2 + t1);
            // printf("push : %d\n",  t2 + t1);
            icp += 5;
            continue;

        case OpSub:
            t1 = pop(&stack);
            t2 = pop(&stack);
            printf("%d\n", t2 - t1);
            push(&stack, t2 - t1);
            // printf("push : %d\n",  t2 - t1);
            icp += 5;
            continue;

        case OpMul:
            t1 = pop(&stack);
            t2 = pop(&stack);
            printf("%d\n", t2 * t1);
            push(&stack, t2 * t1);
            // printf("push : %d\n",  t2 * t1);
            icp += 5;
            continue;

        case OpDiv:
            // TODO: 0除算などを考慮すべき
            t1 = pop(&stack);
            t2 = pop(&stack);
            printf("%d\n", t2 / t1);
            push(&stack, t2 / t1);
            // printf("push : %d\n",  t2 / t1);
            icp += 5;
            continue;

        case OpPush:
            push(&stack, (*icp[1]));
            // printf("push : %d\n",  (*icp[1]));
            icp += 5;
            continue;
        
        case OpEnd:
            return;

        default:
            return;
        }
    }
}