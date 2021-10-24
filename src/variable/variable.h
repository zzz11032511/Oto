#pragma once

#include <stdint.h>

#include "type.h"
#include "../token/token.h"
#include "../token/tokencode.h"

typedef struct var {
    type_t type;
    tokencode_t tc;  // 割り当てられたトークンコード
    union value_u {
        int64_t iVal;
        double  fVal;
        void   *pVal;
    } value;
} Var;


Var *get_var_pointer(tokencode_t tc);

/* get_var_pointer()を短縮したもの */
#define VAR_P(tc) get_var_pointer(tc)

type_t get_type(tokencode_t tc);
int64_t get_ivalue(tokencode_t tc);
double get_fvalue(tokencode_t tc);
void *get_pvalue(tokencode_t tc);

void assign_int(tokencode_t tc, type_t type, int64_t value);
void assign_float(tokencode_t tc, type_t type, double value);
void assign_pointer(tokencode_t tc, type_t type, void *value);

/* ヒープに置かれた変数(Sound, Filter)を解放する */
void free_var();