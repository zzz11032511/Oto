#pragma once

#include <stdint.h>

enum {
    TyVoid = 0,
    TyConstI,
    TyConstF,
    TyInt,
    TyFloat
};

union value_u {
    int64_t  iVal;
    uint64_t uiVal;
    double   dVal;
    void *pVal;
}

typedef struct var {
    int32_t type;       // 型
    union value_u value;    // 中身
} var_t;