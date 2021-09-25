#pragma once

#include <stdint.h>

#include "../token/tokencode.h"

#define IS_RSVWORD(tc)        ((TcBegin <= tc) && (tc <= TcEnd))
#define IS_VALID_NAME(tc)     (tc > TcExit)
#define IS_SYMBOL(tc)         ((TcLF <= tc) && (tc < TcBegin))
#define IS_NOT_SYMBOL(tc)     (tc >= TcBegin)
#define IS_COMMAND(tc)        ((TcPrint <= tc) && (tc <= TcPlay))
#define IS_ARITH_OPERATOR(tc) ((TcPlus <= tc && tc <= TcGt) || (TcAnd <= tc && tc <= TcOr))

/* トークンのパターン識別子 */ 
#define PtnLabel        -1    // 変数かラベル
#define PtnExpr         -2    // 式
#define PtnInst         -3    // 命令
#define PtnStop         -99   // 都合により評価を止めたいとき

void parser_sub(uint32_t *icp, uint32_t start, uint32_t end);

void parser_if(uint32_t *cur, uint32_t *icp);
void parser_loop(uint32_t *cur, uint32_t *icp);
void parser_instruction(uint32_t *cur, uint32_t *icp);
void parser_args(uint32_t *icp, uint32_t start, uint32_t end, uint32_t max_of_params);
void parser_connect_filters(uint32_t *cur, uint32_t *icp);

void expr(uint32_t *cur, uint32_t end, uint32_t *icp);

/**
 * startを始まりとしたブロック(BEGIN~END, (~), [~])の終端の場所を返す 
 * 返される場所は(END, ), ])の位置
 */
uint32_t seek_block_end(uint32_t start);

uint32_t seek_args_block_end(uint32_t start, tokencode_t end_tc);