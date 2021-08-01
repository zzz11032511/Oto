#pragma once

#include <stdint.h>

enum TYPE {
    TyVoid = 0,
    TyConstI,
    TyConstF,
    TyFloat,
    TyChannel,
    TySound,
    TyFilter,
    TyFile,
    TyINDevice,
    TyOUTDevice,
};

typedef struct var {
    int32_t type;      // 型
    union value_u {    // 中身
        int64_t iVal;
        double  fVal;
        struct var *pVal;
    } value;
    int32_t tc;    // 割り当てられたトークンコード
} var_t;