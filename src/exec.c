#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "util.h"
#include "ic.h"
#include "vStack.h"
#include "variable.h"
#include "alu.h"

void printVal(var_t var) {
    switch (var.type) {
    case TyInt:
        printf("%d\n", var.value.iVal);
        break;
    case TyConstI:
        printf("%d\n", var.value.iVal);
        break;
    case TyFloat:
        printf("%f\n", var.value.fVal);
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

    // 計算用の一時変数
    var_t t1;
    var_t t2;
    var_t t3;

    while (1) {
        // printf("opcode : %d\n", (int32_t)icp[0]);
        switch ((int32_t) icp[0]) {
        case OpDef:
            icp[2]->type = (int32_t)icp[1];
            icp[2]->value.iVal = icp[3]->value.iVal;    // intだろうがfloatだろうがこれで代入できる

            // printf("type : %d, value : %d\n", (int32_t)icp[1], icp[3]->value.iVal);
            icp += 5;
            continue;

        case OpCpyS:
            icp[1]->type = icp[2]->type;
            icp[1]->value.iVal = icp[2]->value.iVal;
            icp += 5;
            continue;

        case OpCpyP:
            t1 = pop(&stack);
            icp[1]->type = t1.type;
            icp[1]->value.iVal = t1.value.iVal;
            icp += 5;
            continue;

        case OpAdd:
            t1 = pop(&stack);
            t2 = pop(&stack);
            
            t3 = add2(t2, t1);
            
            push(&stack, t3);
            icp += 5;
            continue;

        case OpSub:
            t1 = pop(&stack);
            t2 = pop(&stack);
            
            t3 = sub2(t2, t1);
            
            push(&stack, t3);
            icp += 5;
            continue;

        case OpMul:
            t1 = pop(&stack);
            t2 = pop(&stack);

            t3 = mul2(t2, t1);

            push(&stack, t3);
            icp += 5;
            continue;

        case OpDiv:
            t1 = pop(&stack);
            t2 = pop(&stack);

            t3 = div2(t2, t1);
            
            push(&stack, t3);
            icp += 5;
            continue;

        case OpPush:
            push(&stack, *icp[1]);
            printf("push : %d\n", (*icp[1]).value.iVal);

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