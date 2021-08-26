#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "../opcode.h"
#include "../../error/error.h"
#include "../../utils/util.h"
#include "../../variable/variable.h"

var_t calculation(var_t v1, var_t v2, uint32_t opcode) {
    var_t t;

    switch (opcode) {
    case OpAdd:
    case OpAdd2:
        t.value.fVal = v1.value.fVal + v2.value.fVal;
        break;
    case OpSub:
    case OpSub2:
        t.value.fVal = v1.value.fVal - v2.value.fVal;
        break;
    case OpMul:
    case OpMul2:
        t.value.fVal = v1.value.fVal * v2.value.fVal;
        break;
    case OpDiv:
    case OpDiv2:
        t.value.fVal = v1.value.fVal / v2.value.fVal;
        break;
    case OpMod:
    case OpMod2:
        if ((int64_t)v2.value.fVal == 0) {
            call_exception(ZERO_DIVISION_EXCEPTION);
        }
        t.value.fVal = (double)((int64_t)v1.value.fVal % (int64_t)v2.value.fVal);
        break;
    case OpAnd:
        t.type = TyConst;
        t.value.iVal = v1.value.iVal & v2.value.iVal;
        break;
    case OpOr:
        t.type = TyConst;
        t.value.iVal = v1.value.iVal | v2.value.iVal;
        break;
    default:
        call_exception(EXCEPTION);
    }

    return t;
}

int32_t compare(var_t v1, var_t v2, uint32_t opcode) {
    int64_t v1Val;
    int64_t v2Val;

    v1Val = (int64_t)v1.value.fVal;
    v2Val = (int64_t)v2.value.fVal;

    switch (opcode) {
    case OpEq:
        return v1Val == v2Val;
    case OpNEq:
        return v1Val != v2Val;
    case OpLtCmp:
        return v1Val <  v2Val;
    case OpLtEqCmp:
        return v1Val <= v2Val;
    case OpRiCmp:
        return v1Val >  v2Val;
    case OpRiEqCmp:
        return v1Val >= v2Val;
    default:
        call_exception(EXCEPTION);
    }

    return 0;
}
