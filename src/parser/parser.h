#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

/* トークン比較関連のマクロ */
#define IS_RSVWORD(tc)        (TcBegin <= tc) && (tc <= TcEnd)
#define IS_VALID_NAME(tc)     tc > TcExit
#define IS_SYMBOL(tc)         (TcLF <= tc) && (tc < TcBegin)
#define IS_NOT_SYMBOL(tc)     tc >= TcBegin
#define IS_ARITH_OPERATOR(tc) (TcPlus <= tc) && (tc < TcGt)
#define IS_OPERATION(tc)      ((TcPlus <= tc && tc <= TcGt) || (TcAnd <= tc && tc <= TcOr))


/* compile.c */
/* ic[]に書き込むための関数 */
void put_ic(var_t **ic, uint32_t *icp, uint32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4);
/* token列を内部コードに変換する */
void compile_sub(tokenbuf_t *tcbuf, var_t *var_list, var_t **ic, uint32_t *icp, uint32_t start, uint32_t end);
/* 命令の引数部分をコンパイルする */
void compile_args(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp, uint32_t num_of_params);
/* 命令をコンパイルする */
void compile_statement(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp);

/* control.c */
/* "begin", '['の位置を渡すと、それに対応した"end", ']'の位置を返す */
uint32_t search_block_end(tokenbuf_t *tcbuf, uint32_t pc);
void loop_control(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp);
void if_control(tokenbuf_t *tcbuf, uint32_t *pc, var_t *var_list, var_t **ic, uint32_t *icp);

/* expr.c */
void expr(tokenbuf_t *tcbuf, uint32_t *pc, uint32_t end, var_t *var_list, var_t **ic, uint32_t *icp);
