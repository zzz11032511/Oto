#pragma once

#include <oto/oto.h>

extern char *src;
extern VectorPTR *vars;
extern Status *oto_status;

/* 変数を取得するための便利マクロ */
#define VAR(tc)  ((Var *)(vars->data[tc]))

/* 内部コードを出力するための便利関数 */
void put_opcode(int64_t *icp, opcode_t op, Var *v1, Var *v2, Var *v3, Var *v4);

void compile_sub(int64_t *icp, SliceI64 *srctcs, int64_t start, int64_t end);
void compile_loop(int64_t *icp, SliceI64 *srctcs, int64_t *idx);
void compile_if(int64_t *icp, SliceI64 *srctcs, int64_t *idx);
void compile_expr(int64_t *icp, SliceI64 *exprtcs, VectorPTR *vars);
void compile_args(int64_t *icp, SliceI64 *argtcs, int64_t max_params);
void compile_instruction(int64_t *icp, SliceI64 *srctcs, int64_t *idx);
void compile_conn_filter(int64_t *icp, SliceI64 *conntcs);
void compile_array(int64_t *icp, SliceI64 *srctcs, int64_t *idx);

SliceI64 *make_line_tokencodes(SliceI64 *srctcs, int64_t start);
SliceI64 *make_args_enclosed_br(SliceI64 *srctcs, int64_t sqbropn);
SliceI64 *make_begin_end_block(SliceI64 *srctcs, int64_t begin);

SliceI64 *make_ifthen_block(SliceI64 *srctcs, int64_t then);

void print_slice_srcs(SliceI64 *srctcs);

void error_compiler(errorcode_t err, SliceI64 *srctcs, int64_t idx);

// トークンコード列でのidxをソース文字列でのidxに変換する
int64_t tc2srcidx(SliceI64 *srctcs, int64_t tcidx);