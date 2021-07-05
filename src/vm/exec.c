#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "alu.h"
#include "ic.h"
#include "../utils/util.h"
#include "../variable/vStack.h"
#include "../variable/variable.h"

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

void exec(var_t **ic, var_t *var) {
    var_t **icp = ic;
    var_t **base = ic;

    // 計算用のスタック
    struct vStack stack;
    stack.sp = 0;

    // 計算用の一時変数
    var_t t1;
    var_t t2;
    var_t t3;

    while (1) {
        // printf("opcode : %d\n", (int32_t)icp[0]);
        switch ((int64_t)icp[0]) {
            case OpDef:
                icp[2]->type = (int32_t)((int64_t)icp[1]);
                icp[2]->value.iVal =icp[3]->value.iVal;  // intだろうがfloatだろうがこれで代入できる

                // printf("type : %d, value : %d\n", (int32_t)icp[1],
                // icp[3]->value.iVal);
                icp += 5;
                continue;

            case OpCpyS:
                icp[1]->type       = icp[2]->type;
                icp[1]->value.iVal = icp[2]->value.iVal;
                icp += 5;
                continue;

            case OpCpyP:
                t1                 = pop(&stack);
                icp[1]->type       = t1.type;
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

            case OpMod:
                t1 = pop(&stack);
                t2 = pop(&stack);
                t3 = mod2(t2, t1);
                push(&stack, t3);
                icp += 5;
                continue;

            case OpAdd1:
                icp[1]->value.iVal += 1;
                icp += 5;
                continue;

            case OpSub1:
                icp[1]->value.iVal -= 1;
                icp += 5;
                continue;

            case OpPush:
                push(&stack, *icp[1]);
                // printf("push : %d\n", (*icp[1]).value.iVal);
                icp += 5;
                continue;

            // 比較命令
            case OpEq:
            case OpNEq:
            case OpLtCmp:
            case OpLtEqCmp:
            case OpRiCmp:
            case OpRiEqCmp:
                t1 = pop(&stack);
                t2 = pop(&stack);
                t3.type = TyConstI;
                t3.value.iVal = compare(t2, t1, (int64_t)icp[0]);
                push(&stack, t3);
                icp += 5;
                continue;

            case OpAnd:
                t1 = pop(&stack);
                t2 = pop(&stack);
                t3.type = TyConstI;
                t3.value.iVal = t1.value.iVal * t1.value.iVal;
                push(&stack, t3);
                icp += 5;
                continue;

            case OpOr:
                t1 = pop(&stack);
                t2 = pop(&stack);
                t3.type = TyConstI;
                t3.value.iVal = t1.value.iVal + t1.value.iVal;
                push(&stack, t3);
                icp += 5;
                continue;

            case OpJmp:
                icp = base + (int64_t)icp[1];
                continue;

            case OpJz:
                t1 = pop(&stack);
                if (t1.value.iVal == 0) {
                    icp = base + (int64_t)icp[1];
                } else {
                    icp += 5;
                }
                continue;

            case OpJnz:
                t1 = pop(&stack);
                if (t1.value.iVal != 0) {
                    icp = base + (int64_t)icp[1];
                } else {
                    icp += 5;
                }
                continue;

            case OpPrint:
                printVal(*icp[1]);
                icp += 5;
                continue;

            case OpEnd:
                return;

            default:
                return;
        }
    }
}