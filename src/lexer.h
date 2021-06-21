#ifndef INCLUDED_LEXER
#define INCLUDED_LEXER 1

#include "util.h"
#include "token.h"
#include "variable.h"

int lexer(String s, tokenBuf_t *tcBuf, var_t **var);

#endif