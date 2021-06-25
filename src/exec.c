#include <stdio.h>
#include <stdint.h>
#include "util.h"
#include "ic.h"
#include "vStack.h"
#include "variable.h"

void printVal(var_t var) {
    switch (var.type) {
    case TyInt:
        printf("%d\n", var.value.iVal);
        break;
    case TyConstI:
        printf("%d\n", var.value.iVal);
        break;
    case TyFloat:
        printf("%d\n", var.value.fVal);
        break;
    case TyConstF:
        printf("%f\n", var.value.fVal);
        break;    
    default:
        break;
    }
    return;
}

void exec(var_t **ic, var_t *var)
{
    var_t **icp = ic;

    // 計算用のスタック
    struct vStack stack;
    stack.sp = 0;

    // 演算用の一時変数
    var_t t1;
    var_t t2;
    var_t t3;

    while (1) {
        printf("opcode : %d\n", (int32_t)icp[0]);
        switch ((int32_t) icp[0]) {
        case OpCpy:
            continue;

        case OpAdd:
            t1 = pop(&stack);
            t2 = pop(&stack);
            
            t3.type = TyInt;
            t3.value.iVal = t2.value.iVal + t1.value.iVal;
            
            push(&stack, t3);
            icp += 5;
            continue;

        case OpSub:
            t1 = pop(&stack);
            t2 = pop(&stack);
            
            t3.type = TyInt;
            t3.value.iVal = t2.value.iVal - t1.value.iVal;
            
            push(&stack, t3);
            icp += 5;
            continue;

        case OpMul:
            t1 = pop(&stack);
            t2 = pop(&stack);
            
            t3.type = TyInt;
            t3.value.iVal = t2.value.iVal * t1.value.iVal;
            
            push(&stack, t3);
            icp += 5;
            continue;

        case OpDiv:
            // TODO: 0除算などを考慮すべき
            t1 = pop(&stack);
            t2 = pop(&stack);
            
            if (t1.value.iVal == 0) {
                fprintf(stderr, "Zero division error!\n");
                exit(1);
            }

            t3.type = TyInt;
            t3.value.iVal = t2.value.iVal / t1.value.iVal;
            
            push(&stack, t3);
            icp += 5;
            continue;

        case OpPush:
            push(&stack, *icp[1]);
            // printf("push : %d\n", (*icp[1]).value.iVal);

            icp += 5;
            continue;

        case OpPrint:
            t1 = peek(&stack);
            printVal(t1);
            icp += 5;
            continue;
        
        case OpEnd:
            return;

        default:
            return;
        }
    }
}