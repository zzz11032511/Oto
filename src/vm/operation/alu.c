#include <stdio.h>
#include <stdint.h>

#include "../../opcode.h"
#include "../../error/error.h"
#include "../../variable/variable.h"
#include "../../variable/type.h"

#define IS_JUST_ZERO(n) (n & 0xffffffff) == 0

Var calculation(Var v1, Var v2, opcode_t opcode) {
    Var t;

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
        if (IS_JUST_ZERO(v2.value.iVal)) {
            // 完全に0なら0除算とする
            call_error(ZERO_DIVISION_ERROR);
        }
        t.value.fVal = v1.value.fVal / v2.value.fVal;
        break;
    case OpMod:
    case OpMod2:
        if ((int64_t)v2.value.fVal == 0) {
            call_error(ZERO_DIVISION_ERROR);
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
        call_error(UNKNOWN_ERROR, "calculation()");
    }

    return t;
}

int32_t compare(Var v1, Var v2, uint32_t opcode) {
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
        call_error(UNKNOWN_ERROR, "compare()");
    }

    return 0;
}
