#pragma once

#include <oto.h>

extern SliceI64 *srctcs;
extern VectorPTR *vars;
extern VectorPTR *ops;

/* トークンや変数を取得するための便利マクロ */
#define SRC(idx) slice_i64_get(srctcs, idx)
#define VAR(tc)  ((Var *)(vars->data[tc]))

void put_opcode(int64_t *icp, opcode_t op, Var *v1, Var *v2, Var *v3, Var *v4);

void compile_sub(int64_t *icp, int64_t start, int64_t end);
void compile_loop(int64_t *icp, int64_t *idx);
void expr(int64_t *icp, SliceI64 *exprtcs, VectorPTR *vars);

SliceI64 *make_line_tokencodes(SliceI64 *srctcs, int64_t start);
SliceI64 *make_args_enclosed_br(SliceI64 *srctcs, int64_t sqbropn);
SliceI64 *make_begin_end_block(SliceI64 *srctcs, int64_t begin);
