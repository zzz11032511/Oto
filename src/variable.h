#pragma once

#include <stdint.h>

enum TYPE {
    TyVoid = 0,
    TyConstI,
    TyConstF,
    TyInt,
    TyFloat
};

typedef struct var {
    int32_t type;      // 型
    union value_u {    // 中身
        int64_t  iVal;
        uint64_t uiVal;
        double   fVal;
        void *pVal;
    } value;
} var_t;