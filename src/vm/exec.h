#pragma once

#include "../lexer/token.h"
#include "../variable/variable.h"

void exec(var_t **ic, var_t *var, tokenbuf_t *tcbuf);