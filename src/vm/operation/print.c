#include <stdio.h>
#include <stdint.h>

#include "../../error/error.h"
#include "../../variable/variable.h"
#include "../../variable/var_stack.h"
#include "../../variable/type.h"

void print_var(struct var_stack *stack) {
    var_t t = vpop(stack);

    switch (t.type) {
    case TyFloat:
    case TyConst:
        printf("%lf\n", t.value.fVal);
        break;
    default:
        call_error(UNKNOWN_ERROR, "print_var()");
    }
}
