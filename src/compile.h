#ifndef INCLUDED_COMPILE
#define INCLUDED_COMPILE 1

#include "util.h"

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

int compile(String s, int *tc, int *var);

#endif