#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#include "../../sound/sound.h"
#include "../../variable/type.h"
#include "../../variable/var_stack.h"
#include "../../variable/variable.h"

void beep(struct var_stack *stack) {
    var_t t1 = vpop(stack);  // length
    var_t t2 = vpop(stack);  // frequency

    if (t1.type == TyInitVal) {
        t1.value.fVal = 1;
    }

    if (t2.type == TyInitVal) {
        t2.value.fVal = 524;
    }

    printf("[Beep] ");
    printf("frequency : %8.3f[Hz], length : %2.2f[s]\n", 
           t2.value.fVal, t1.value.fVal * 1000);

    Beep((uint64_t)t2.value.fVal, (uint64_t)(t1.value.fVal * 1000));
}

void define_sound(var_t *v1, var_t *v2) {
    v1->value.pVal = (void *)new_sound((int32_t)v2->value.fVal, NULL, 0);
}