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
    uint32_t type;
    union value_u {
        int64_t iVal;
        double  fVal;
        struct var *pVal;
    } value;
    uint32_t tc;    // 割り当てられたトークンコード
} var_t;