#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#include "../../variable/var_stack.h"
#include "../../variable/variable.h"

void beep(struct var_stack *stack) {
    var_t t1 = vpop(stack);  // length
    var_t t2 = vpop(stack);  // frequency

    if (t1.type == TyInitVal) {
        t1.value.fVal = 500;
    }

    if (t2.type == TyInitVal) {
        t2.value.fVal = 1;
    }

    Beep((uint64_t)t2.value.fVal, (uint64_t)(t1.value.fVal * 1000));
}