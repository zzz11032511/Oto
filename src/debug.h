#pragma once

#include <stdint.h>
#include "utils/util.h"
#include "lexer/token.h"
#include "variable/variable.h"

/* 内部コードを出力する */
void printIc(var_t **ic, int32_t *icp, int32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4);

/* ソースコードをトークンに変換したものを出力する */
void printTokenCode(tokenBuf_t *tcBuf, int32_t length);

/* 数式を表すトークンをRPNにしたものを出力する */
void printRpnTc(tokenBuf_t *tcBuf, int32_t *rpnTc, int32_t length);