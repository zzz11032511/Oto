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
void putIc(int32_t **ic, int32_t *icp, int32_t op, int32_t *v1, int32_t *v2, int32_t *v3, int32_t *v4);

/* 内部コードに変換する関数 */
int32_t compile(str_t s, tokenBuf_t *tcBuf, var_t **var, int32_t **ic);