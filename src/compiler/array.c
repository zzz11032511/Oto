#include "compiler.h"

void compile_array(int64_t *icp, SliceI64 *srctcs, int64_t *idx) {
    tokencode_t var_tc = slice_i64_get(srctcs, *idx);
    *idx += 2;

    SliceI64 *slice = make_args_enclosed_br(srctcs, *idx);
    DEBUG_IPRINT(slice->length);
    if (slice->length % 2 == 0) {
        // 配列の書き方が違うと思われるのでエラー
        oto_error(OTO_INVALID_SYNTAX_ERROR);
    }

    int64_t size = 1 + ((slice->length - 1) / 2);
    DEBUG_IPRINT(size);
    compile_args(icp, slice, size);

    put_opcode(icp, OP_ARRAYDEF, VAR(var_tc), (Var *)size, 0, 0);

    *idx += slice->length + 2;
    DEBUG_IPRINT(*idx);
}