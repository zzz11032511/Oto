#include <oto.h>

#include "compiler.h"

void compile_loop(int64_t *icp, SliceI64 *srctcs, int64_t *idx) {
    // ループ回数の位置
    int64_t idx2 = *idx + 2;
    Var *loop_cnt = VAR(slice_i64_get(srctcs, idx2));
    idx2 += 2;

    int64_t jmp_icp = *icp;
    put_opcode(icp, OP_LOOP, 0, 0, 0, 0);

    SliceI64 *slice = make_begin_end_block(srctcs, idx2);
    compile_sub(icp, slice, 0, slice->length);

    put_opcode(icp, OP_JMP, (Var *)jmp_icp, 0, 0, 0);
    put_opcode(&jmp_icp, OP_LOOP, ((Var *)*icp), loop_cnt, 0, 0);

    *idx = idx2 + slice->length;

    free_slice_i64(slice);
}

void compile_if(int64_t *icp, SliceI64 *srctcs, int64_t *idx) {
    int64_t idx2 = *idx + 1;

    bool not_flag = false;
    if (slice_i64_get(srctcs, idx2) == TC_NOT) {
        not_flag = true;
        idx2++;
    }

    // 引数(条件式)のコンパイル
    SliceI64 *slice = make_args_enclosed_br(srctcs, idx2);
    compile_expr(icp, slice, vars);
    idx2 += slice->length + 2;
    free_slice_i64(slice);

    int64_t jmp_icp = *icp;
    put_opcode(icp, OP_JZ, 0, 0, 0, 0);

    slice = make_ifthen_block(srctcs, idx2);
    compile_sub(icp, slice, 0, slice->length);
    idx2 += slice->length + 1;
    free_slice_i64(slice);

    int64_t jmp_icp2 = *icp;
    put_opcode(icp, OP_JMP, 0, 0, 0, 0);

    if (not_flag) {
        put_opcode(&jmp_icp, OP_JNZ, ((Var *)*icp), 0, 0, 0);
    } else {
        put_opcode(&jmp_icp, OP_JZ, ((Var *)*icp), 0, 0, 0);
    }

    if (slice_i64_get(srctcs, idx2) == TC_ELSIF) {
        compile_if(icp, srctcs, &idx2);

    } else if (slice_i64_get(srctcs, idx2) == TC_ELSE) {
        slice = make_begin_end_block(srctcs, idx2);

        compile_sub(icp, slice, 0, slice->length);
        idx2 += slice->length;
        free_slice_i64(slice);

    } else {
        idx2++;

    }

    put_opcode(&jmp_icp2, OP_JMP, ((Var *)*icp), 0, 0, 0);
    *idx = idx2;
}
