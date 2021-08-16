#pragma once

#include <stdint.h>
#include "../sound/sound.h"

enum TYPE {
    TyVoid = 0,
    TyConst,
    TyFloat,
    TySound
};

typedef struct var {
    uint32_t type;
    union value_u {
        int64_t iVal;
        double  fVal;
        void   *pVal;
    } value;
    uint32_t tc;    // 割り当てられたトークンコード
} var_t;

void init_var_list(var_t *var_list, uint32_t size);
void free_heap_var(var_t *var_list, uint32_t size);
