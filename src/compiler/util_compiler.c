#include <oto.h>

#include "compiler.h"

SliceI64 *make_line_tokencodes(SliceI64 *srctcs, int64_t start) {
    int64_t end = start;
    while (srctcs->data[end] != TC_LF) {
        end++;
        if (end >= srctcs->length) {
            oto_error_exit(OTO_INVALID_SYNTAX_ERROR);
        }
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    return slice;
}

SliceI64 *make_args_enclosed_br(SliceI64 *srctcs, int64_t sqbropn) {
    int64_t start = sqbropn + 1;
    int64_t end = sqbropn;

    int64_t nest = 0;
    for (;;) {
        tokencode_t tc = srctcs->data[end];

        if (tc == TC_SQBROPN || tc == TC_BROPN) {
            nest++;
        } else if  (tc == TC_SQBRCLS || tc == TC_BRCLS) {
            nest--;
        }
        
        if ((tc == TC_SQBRCLS || TC_BRCLS) && nest == 0) {
            break;
        } else if (end >= srctcs->length) {
            oto_error_exit(OTO_INVALID_SYNTAX_ERROR);
        }

        end++;
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    return slice;
}

SliceI64 *make_begin_end_block(SliceI64 *srctcs, int64_t begin) {
    int64_t start = begin + 1;
    int64_t end = begin + 1;

    int64_t nest = 1;
    for (;;) {
        tokencode_t tc = srctcs->data[end];

        /* ifの個数とifブロックのendの個数は一致する */
        if (tc == TC_BEGIN || tc == TC_IF) {
            nest++;
        } else if  (tc == TC_END) {
            nest--;
        }

        if (tc == TC_END && nest == 0) {
            break;
        } else if (end >= srctcs->length) {
            oto_error_exit(OTO_INVALID_SYNTAX_ERROR);
        }

        end++;
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    return slice;
}

/* then ~ elsif, elseのばしょを返す */
SliceI64 *make_ifthen_block(SliceI64 *srctcs, int64_t then) {
    int64_t start = then + 1;
    int64_t end = then;

    for (;;) {
        tokencode_t tc = srctcs->data[end];

        // ブロック内のif-end, begin-endを飛ばす
        if (tc == TC_IF || tc == TC_BEGIN) {
            int64_t nest = 0;
            for (;;) {
                tc = srctcs->data[end];
                if (tc == TC_IF || tc == TC_BEGIN) {
                    nest++;
                } else if (tc == TC_END) {
                    nest--;
                }

                if (tc == TC_END && nest == 0) {
                    break;
                } else if (end >= srctcs->length) {
                    oto_error_exit(OTO_INVALID_SYNTAX_ERROR);
                }  
                end++;
            }
        } else if (tc == TC_ELSIF || tc == TC_ELSE || tc == TC_END) {
            break;
        } else if (end >= srctcs->length) {
            oto_error_exit(OTO_INVALID_SYNTAX_ERROR);
        }

        end++;
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    return slice;
}

void compile_args(int64_t *icp, SliceI64 *argtcs, int64_t max_params) {
    int64_t idx = 0;
    int64_t params = 0;
    tokencode_t tc = slice_i64_get(argtcs, idx);

    while (tc != TC_LF) {
        int64_t start = idx;
        int64_t end   = idx;
        while (slice_i64_get(argtcs, end) != TC_COMMA 
               && slice_i64_get(argtcs, end) != TC_LF) {
            end++;
        }

        if (end - start == 1) {
            put_opcode(icp, OP_PUSH, VAR(tc), 0, 0, 0);
            idx += 1;

        } else if (end - start == 0) {
            put_opcode(icp, OP_PUSH_INITVAL, 0, 0, 0, 0);

        } else {
            // 引数一つのコンパイル
            SliceI64 *slice = new_slice_i64_from_slice(argtcs, start, end);
            compile_expr(icp, slice, vars);
            idx += slice->length;
            free_slice_i64(slice);          
        }

        params++;
        idx++;
        tc = slice_i64_get(argtcs, idx);
    }

    if (params > max_params) {
        oto_error_exit(OTO_TOO_MANY_ARGUMENTS_ERROR);
    }

    while (params < max_params) {
        put_opcode(icp, OP_PUSH_INITVAL, 0, 0, 0, 0);
        params++;
    }
}

void print_slice_srcs(SliceI64 *srctcs) {
    printf("\n[slice]\n");

    for (int64_t i = 0; i < srctcs->length; i++) {
        printf("%I64d : %s\n", i, VAR(slice_i64_get(srctcs, i))->token->str);
    }
    printf("\n");
}
