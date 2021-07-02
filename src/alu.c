#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "ic.h"
#include "util.h"
#include "variable.h"

var_t add2(var_t v1, var_t v2) {
    var_t t;
    // printf("type : %d, %d\n", v1.type, v2.type);
    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal + v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal + v2.value.fVal;
            t.type       = TyConstF;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal + (double)v2.value.iVal;
            t.type       = TyConstF;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal + v2.value.fVal;
            t.type       = TyConstF;
        }
    }

    return t;
}

var_t sub2(var_t v1, var_t v2) {
    var_t t;

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal - v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal - v2.value.fVal;
            t.type       = TyConstF;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal - (double)v2.value.iVal;
            t.type       = TyConstF;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal - v2.value.fVal;
            t.type       = TyConstF;
        }
    }

    return t;
}

var_t mul2(var_t v1, var_t v2) {
    var_t t;

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal * v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal * v2.value.fVal;
            t.type       = TyConstF;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal * (double)v2.value.iVal;
            t.type       = TyConstF;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal * v2.value.fVal;
            t.type       = TyConstF;
        }
    }

    return t;
}

var_t div2(var_t v1, var_t v2) {
    var_t t;

    if (v1.value.iVal == 0) {
        fprintf(stderr, "Zero division error.\n");
        exit(1);
    }

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal / v2.value.iVal;
            t.type       = TyConstI;
        } else {
            // int + float
            t.value.fVal = (double)v1.value.iVal / v2.value.fVal;
            t.type       = TyConstF;
        }
    } else {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + float
            t.value.fVal = v1.value.fVal / (double)v2.value.iVal;
            t.type       = TyConstF;
        } else {
            // float + float
            t.value.fVal = v1.value.fVal / v2.value.fVal;
            t.type       = TyConstF;
        }
    }
    return t;
}

var_t mod2(var_t v1, var_t v2) {
    var_t t;

    if (v1.value.iVal == 0) {
        fprintf(stderr, "zero division error.\n");
        exit(1);
    }

    if (v1.type == TyInt || v1.type == TyConstI) {
        if (v2.type == TyInt || v2.type == TyConstI) {
            // int + int
            t.value.iVal = v1.value.iVal % v2.value.iVal;
            t.type       = TyConstI;
        } else {
            fprintf(stderr, "float type cannot modulo.\n");
            exit(1);
        }
    } else {
        fprintf(stderr, "float type cannot modulo.\n");
        exit(1);
    }

    return t;
}

int32_t compare(var_t v1, var_t v2, int32_t opcode) {
    int64_t v1Val;
    int64_t v2Val;

    if (v1.type == TyFloat || v1.type == TyConstF) {
        v1Val = (int64_t)v1.value.fVal;
    } else {
        v1Val = v1.value.iVal;
    }

    if (v2.type == TyFloat || v2.type == TyConstF) {
        v2Val = (int64_t)v2.value.fVal;
    } else {
        v2Val = v2.value.iVal;
    }

    switch (opcode) {
        case OpEq:
            return v1Val == v2Val;
        case OpNEq:
            return v1Val != v2Val;
        case OpLtCmp:
            return v1Val < v2Val;
        case OpLtEqCmp:
            return v1Val <= v2Val;
        case OpRiCmp:
            return v1Val > v2Val;
        case OpRiEqCmp:
            return v1Val >= v2Val;
        default:
            return 0;
    }
}