#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "alu.h"
#include "ic.h"
#include "../errorHandle.h"
#include "../debug.h"
#include "../utils/util.h"
#include "../variable/vStack.h"
#include "../variable/variable.h"

void printVal(var_t var) {
    switch (var.type) {
        case TyInt:
            printf("%ld\n", var.value.iVal);
            break;
        case TyConstI:
            printf("%ld\n", var.value.iVal);
            break;
        case TyFloat:
            printf("%.3lf\n", var.value.fVal);
            break;
        case TyConstF:
            printf("%.3lf\n", var.value.fVal);
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

#ifdef DEBUG
    printIc(icp);
#endif

    while (1) {
        switch ((int64_t)icp[0]) {
            case OpDef:
                icp[2]->type = (int32_t)((int64_t)icp[1]);
                
                // intだろうがfloatだろうがこれで代入できる
                icp[2]->value.iVal = icp[3]->value.iVal;  

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

            case OpPush:
                push(&stack, *icp[1]);
                icp += 5;
                continue;

            case OpAdd:
            case OpSub:
            case OpMul:
            case OpDiv:
            case OpMod:
            case OpAnd:
            case OpOr:
                t1 = pop(&stack);
                t2 = pop(&stack);
                t3 = calculation(t2, t1, (int64_t)icp[0]);
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
                callException(ERROR);
                return;
        }
    }
}