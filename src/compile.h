#pragma once

#include <stdint.h>
#include "util.h"
#include "token.h"
#include "variable.h"

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

/* 内部コードに変換する関数 */
int32_t compile(str_t s, tokenBuf_t *tcBuf, var_t *var, var_t **ic);