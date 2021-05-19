#ifndef INCLUDED_VARIABLE
#define INCLUDED_VARIABLE 1

#include "util.h"
#include "token.h"

enum {
    TyInt = 0,
    TyFloat
};

typedef struct var {
    int type;       // 型
    String name;    // 変数名
    void *value;    // 中身
    char scope;     // スコープ
} var_t;

#endif