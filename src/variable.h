#ifndef INCLUDED_VARIABLE
#define INCLUDED_VARIABLE 1

#include "util.h"
#include "token.h"

enum {
    TyVoid = 0,
    TyConstI,
    TyConstF,
    TyInt,
    TyFloat
};

typedef struct var {
    int type;       // 型
    int value;    // 中身
} var_t;


/* 未使用の一時変数の確保 */
int tmpAlloc();

/* iが一時変数を指すトークンコードであれば解放する */
void tmpFree(int i);

#endif