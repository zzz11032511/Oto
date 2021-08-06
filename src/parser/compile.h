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
 *      op      : 処理を表す内部コード
 *      v1 ~ v4 : 渡す値(ポインタ)
 */
void put_ic(var_t **ic, uint32_t *icp, uint32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4);

/* token列を内部コードに変換する */
void compile_sub(tokenbuf_t *tcbuf, var_t *var_list, var_t **ic, uint32_t *icp, uint32_t start, uint32_t end);

/* 文字列sを内部コード列にコンパイルする関数 */
void compile(str_t s, tokenbuf_t *tcbuf, var_t *var_list, var_t **ic);