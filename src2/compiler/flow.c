#include <oto.h>

#include "compiler.h"

void compile_loop(int64_t *icp, int64_t *idx) {
    // ループ回数の位置
    int64_t idx2 = *idx + 2;
    Var *loop_cnt = VAR(SRC(idx2));
    idx2 += 2;

    int64_t jmp_icp = *icp;
    put_opcode(icp, OP_LOOP, 0, 0, 0, 0);

    SliceI64 *slice = make_begin_end_block(srctcs, idx2);
    
    /**
     * compile_subの設計上srctcsを対象のスライスに
     * 差し替えてからcompile_sub()を呼び出す必要がある
     * なるべく早くなおす
     */
    SliceI64 *srctcs_org = srctcs;
    srctcs = slice;
    compile_sub(icp, 0, slice->length);
    srctcs = srctcs_org;

    put_opcode(icp, OP_JMP, (Var *)jmp_icp, 0, 0, 0);
    put_opcode(&jmp_icp, OP_LOOP, ((Var *)*icp), loop_cnt, 0, 0);

    *idx = idx2 + slice->length;

    free_slice_i64(slice);
}