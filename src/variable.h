#pragma once

#include <stdint.h>

enum {
    TyVoid = 0,
    TyConstI,
    TyConstF,
    TyInt,
    TyFloat
};

typedef struct var {
    int32_t type;       // 型
    void* value;    // 中身
} var_t;