#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

/**
 * ic[]に書き込むための関数
 * 
 * args:
 *      ic      : 内部コード列
 *      icp     : 現在指しているicのインデックス
 *                中で加算したりするのでポインタを渡してね
 *      op      : 処理を表す内部コード
 *      v1 ~ v4 : 渡す値(ポインタ)
 */
void putIc(var_t **ic, int32_t *icp, int32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4);

/* token列を内部コードに変換する */
void compile_sub(tokenBuf_t *tcBuf, var_t *var, var_t **ic, int32_t *icp, int32_t start, int32_t end);

/* 文字列sを内部コード列にコンパイルする関数 */
int32_t compile(str_t s, tokenBuf_t *tcBuf, var_t *var, var_t **ic);