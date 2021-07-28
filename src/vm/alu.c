#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ic.h"
#include "../errorHandle.h"
#include "../utils/util.h"
#include "../variable/variable.h"

var_t mod2(var_t v1, var_t v2) {
    var_t t;
    t.type = TyConst;
    double v1Val = v1.value.fVal;
    double v2Val = v2.value.fVal;

    if (v2Val < 0) {
        v2Val = -v2Val;
    }

    if (v1Val > 0) {
        while (v1Val - v2Val >= 0) {
            v1Val -= v2Val;
        }
    } else if (v1Val < 0) {
        do {
            v1Val += v2Val;
        } while (v1Val < 0);
    } else {
        callException(ZERO_DIVISION_ERROR);
    }

    t.value.fVal = v1Val;
    return t;
}

var_t calculation(var_t v1, var_t v2, int32_t opcode) {
    var_t t;
    t.type = TyConst;
    switch (opcode) {
    case OpAdd:
        t.value.fVal = v1.value.fVal + v2.value.fVal;
        break;
    case OpSub:
        t.value.fVal = v1.value.fVal - v2.value.fVal;
        break;
    case OpMul:
        t.value.fVal = v1.value.fVal * v2.value.fVal;
        break;
    case OpDiv:
        t.value.fVal = v1.value.fVal / v2.value.fVal;
        break;
    case OpMod:
        t = mod2(v1, v2);
        break;
    case OpAnd:
        t.type = TyConst;
        t.value.iVal = (int32_t)v1.value.fVal & (int32_t)v2.value.fVal;
        break;
    case OpOr:
        t.type = TyConst;
        t.value.iVal = (int32_t)v1.value.fVal | (int32_t)v2.value.fVal;
        break;
    default:
        callException(ERROR);
    }

    return t;
}

int32_t compare(var_t v1, var_t v2, int32_t opcode) {
    int64_t v1Val;
    int64_t v2Val;

    if (v1.type == TyFloat || v1.type == TyConst) {
        v1Val = (int64_t)v1.value.fVal;
    }

    if (v2.type == TyFloat || v2.type == TyConst) {
        v2Val = (int64_t)v2.value.fVal;
    }

    switch (opcode) {
        default:
            return 0;
    }
}