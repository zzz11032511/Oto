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
        case TyFloat:
        case TyConstF:
            printf("%.3lf\n", var.value.fVal);
            break;
        case TyConstI:
            printf("%ld\n", var.value.iVal);
            break;
        default:
            callException(TYPE_ERROR);
    }
    return;
}

void exec(var_t **ic, var_t *var, tokenBuf_t *tcBuf) {
    var_t **icp = ic;
    var_t **base = ic;

    // 計算用のスタック
    struct vStack stack;
    stack.sp = 0;

    // 計算用の一時変数
    var_t t1;
    var_t t2;
    var_t t3;

    int64_t tmp = 0;

#ifdef DEBUG
    printIc(icp, tcBuf);
#endif

    while (1) {
        switch ((int64_t)icp[0]) {
            case OpCpyD:
                icp[1]->type = TyFloat;
                tmp = icp[2]->type;
                if (tmp == TyConstI)
                    icp[1]->value.fVal = (double)icp[2]->value.iVal;
                else 
                    icp[1]->value.fVal = icp[2]->value.fVal;
                
                icp += 5;
                continue;

            case OpCpyP:
                t1 = pop(&stack);
                icp[1]->type = TyFloat;
                tmp = t1.type;
                if (tmp == TyConstI)
                    icp[1]->value.fVal = (double)t1.value.iVal;
                else 
                    icp[1]->value.fVal = t1.value.fVal;
                icp += 5;
                continue;

            case OpPush:
                push(&stack, *icp[1]);
                icp += 5;
                continue;

            // PushC  type, value
            case OpPushC:
                constPush(&stack, (int64_t)icp[1], (int64_t)icp[2]);
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

            case OpLoop:
                tmp = (int64_t)icp[3];
                tmp++;
                icp[3] = (var_t *)tmp;

                int64_t loopCnt = 0;
                if (icp[2]->type == TyConstI) {
                    loopCnt = icp[2]->value.iVal;
                } else {
                    loopCnt = (int64_t)icp[2]->value.fVal;
                }
                if (tmp > loopCnt) {
                    icp = base + (int64_t)icp[1];
                    continue;
                }
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

            case OpExit:
                return;

            default:
                callException(ERROR);
        }
    }
}