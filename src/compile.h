#ifndef INCLUDED_COMPILE
#define INCLUDED_COMPILE 1

#include "util.h"
#include "token.h"
#include "variable.h"

enum {
    OpDef = 0,    // 変数定義
    OpCpy,        // 変数への代入 
    OpAdd,        // 加算
    OpSub,        // 引算
    OpMul,        // 掛算
    OpDiv,        // 割算
    OpPrint,      // 変数の出力
    OpTime,
    OpEnd
};

#define IC_LIST_SIZE 10000

int compile(String s, tokenBuf_t *tcBuf, int *var, int **ic);

#endif