#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "alu.h"
#include "opcode.h"
#include "../error/error.h"
#include "../debug/debug.h"
#include "../utils/util.h"
#include "../variable/var_stack.h"
#include "../variable/variable.h"

#define NEXT_OPERATION(icp) icp += 5; continue;

void print_var(var_t var) {
    switch (var.type) {
        case TyFloat:
        case TyConst:
            printf("%.3lf\n", var.value.fVal);
            break;
        default:
            call_exception(TYPE_ERROR);
    }
    return;
}

void exec(var_t **ic, var_t *var, tokenbuf_t *tcbuf) {
    var_t **icp = ic;
    var_t **base = ic;

    // 計算用のスタック
    struct var_stack stack;
    stack.sp = 0;

    // 計算用の一時変数
    var_t t1;
    var_t t2;
    var_t t3;

    // ループ用の制御変数
    uint64_t loop_var = 0;
    uint64_t loop_cnt = 0;

#ifdef DEBUG
    print_opcodes(tcbuf, icp);
#endif

    while (1) {
        switch ((uint64_t)icp[0]) {
        case OpCpyD:
            icp[1]->type = TyFloat;
            icp[1]->value.fVal = icp[2]->value.fVal;
            NEXT_OPERATION(icp);

        case OpCpyP:
            t1 = vpop(&stack);
            icp[1]->type = TyFloat;
            icp[1]->value.fVal = t1.value.fVal;
            NEXT_OPERATION(icp);

        case OpPush:
            vpush(&stack, *icp[1]);
            NEXT_OPERATION(icp);

        case OpPushC:
            const_vpush(&stack, (uint64_t)icp[1], (uint64_t)icp[2]);
            NEXT_OPERATION(icp);

        case OpAdd:
        case OpSub:
        case OpMul:
        case OpDiv:
        case OpMod:
        case OpAnd:
        case OpOr:
            t1 = vpop(&stack);
            t2 = vpop(&stack);
            t3 = calculation(t2, t1, (uint64_t)icp[0]);
            vpush(&stack, t3);
            NEXT_OPERATION(icp);

        case OpAdd2:
        case OpSub2:
        case OpMul2:
        case OpDiv2:
        case OpMod2:
            icp[1]->type = TyFloat;
            icp[1]->value.fVal = 
                calculation(*icp[2], *icp[3], (uint64_t)icp[0]).value.fVal;
            NEXT_OPERATION(icp);

        case OpEq:
        case OpNEq:
        case OpLtCmp:
        case OpLtEqCmp:
        case OpRiCmp:
        case OpRiEqCmp:
            t1 = vpop(&stack);
            t2 = vpop(&stack);
            t3.value.iVal = compare(t2, t1, (uint64_t)icp[0]);
            vpush(&stack, t3);
            NEXT_OPERATION(icp);

        case OpLoop:
            loop_var = (uint64_t)icp[3];
            loop_var++;
            icp[3] = (var_t *)loop_var;

            loop_cnt = (uint64_t)icp[2]->value.fVal;
            
            if (loop_var > loop_cnt) {
                icp = base + (uint64_t)icp[1];
                continue;
            }
            NEXT_OPERATION(icp);

        case OpJmp:
            icp = base + (uint64_t)icp[1];
            continue;

        case OpJz:
            t1 = vpop(&stack);
            if (t1.value.iVal == 0) {
                icp = base + (uint64_t)icp[1];
                continue;
            }
            NEXT_OPERATION(icp);

        case OpJnz:
            t1 = vpop(&stack);
            if (t1.value.iVal != 0) {
                icp = base + (uint64_t)icp[1];
                continue;
            }
            NEXT_OPERATION(icp);

        case OpPrint:
            print_var(*icp[1]);
            NEXT_OPERATION(icp);

        case OpExit:
            return;

        default:
            call_exception(ERROR);
        }
    }
}