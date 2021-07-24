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
        double   fVal;
        void *pVal;
    } value;
    int32_t tc;    // 割り当てられたトークンコード
} var_t;