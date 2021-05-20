#ifndef INCLUDED_COMPILE
#define INCLUDED_COMPILE 1

#include "util.h"
#include "token.h"
#include "variable.h"

enum {
    OpCpy = 0, 
    OpCeq, 
    OpCne, 
    OpClt,
    OpCge, 
    OpCle, 
    OpCgt, 
    OpAdd, 
    OpSub, 
    OpMul, 
    OpDiv, 
    OpMod, 
    OpAnd, 
    OpAdd1, 
    OpNeg, 
    OpGoto,
    OpJeq,
    OpJne,
    OpJlt,
    OpJge,
    OpJle,
    OpJgt,
    OpLop,
    OpPrint,
    OpTime,
    OpEnd
};

#define IC_LIST_SIZE 10000

int compile(String s, tokenBuf_t *tcBuf, var_t **var, int **ic)
;

#endif