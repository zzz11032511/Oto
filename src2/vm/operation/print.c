#include <stdio.h>
#include <stdint.h>

#include "../../error/error.h"
#include "../../variable/variable.h"
#include "../../variable/type.h"

void print_var(var_t var) {
    switch (var.type) {
    case TyFloat:
    case TyConst:
        printf("%lf\n", var.value.fVal);
        break;
    default:
        call_error(UNKNOWN_ERROR);
    }
}
