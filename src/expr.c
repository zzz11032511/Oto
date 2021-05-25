#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"
#include "compile.h"

int expr(tokenBuf_t tcBuf, int *pc, int *var)
{
    int ppc = *pc; // 最初のpcを保存しておく
}