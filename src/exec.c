#include <stdio.h>
#include "util.h"
#include "ic.h"
#include "vPtrStack.h"
#include "variable.h"

void exec(int **ic, var_t **var)
{
    int **icp = ic;

    // 計算用のスタック
    struct vPtrStack stack;
    stack.sp = 0;
    var_t *t1;    // 演算用の一時変数
    var_t *t2;   

    while (1) {
        // printf("opcode : %d\n", (int)icp[0]);
        switch ((int) icp[0]) {
        case OpCpy:
            continue;

        case OpAdd:
            t1 = pop(&stack);
            t2 = pop(&stack);
            push(&stack, t2 + t1);
            // printf("push : %d\n",  t2 + t1);
            icp += 5;
            continue;

        case OpSub:
            t1 = pop(&stack);
            t2 = pop(&stack);
            push(&stack, t2 - t1);
            icp += 5;
            continue;

        case OpMul:
            t1 = pop(&stack);
            t2 = pop(&stack);
            push(&stack, t2 * t1);
            icp += 5;
            continue;

        case OpDiv:
            // TODO: 0除算などを考慮すべき
            t1 = pop(&stack);
            t2 = pop(&stack);
            push(&stack, t2 / t1);
            icp += 5;
            continue;

        case OpPush:
            push(&stack, icp[1]);
            icp += 5;
            continue;
        
        case OpEnd:
            return;

        default:
            return;
        }
    }
}