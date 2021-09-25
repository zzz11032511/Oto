#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#include "operation/operation.h"
#include "../opcode.h"
#include "../error/error.h"
#include "../debug/debug.h"
#include "../ic/ic.h"
#include "../variable/variable.h"
#include "../variable/var_stack.h"
#include "../variable/type.h"

extern var_t *ic[];

#define NEXT_OPERATION(icp) icp += 5; continue;

void exec() {
    var_t **icp = ic;
    var_t **base = ic;

    struct var_stack stack;
    stack.sp = 0;

    var_t t1;
    var_t t2;
    var_t t3;

    // ループの制御変数
    uint64_t loop_var = 0;
    uint64_t loop_cnt = 0;

    uint64_t samples_per_sec = 40000;

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

        case OpDefS:
            define_sound(icp[1], icp[2]);
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
            print_var(&stack);
            NEXT_OPERATION(icp);

        case OpBeep:
            beep(&stack);
            NEXT_OPERATION(icp);

        case OpPlay:
            play(&stack, samples_per_sec);
            NEXT_OPERATION(icp);

        case OpExit:
            return;

        case OpNop:
            NEXT_OPERATION(icp);

        default:
            call_error(UNKNOWN_ERROR);
        }
    }
}