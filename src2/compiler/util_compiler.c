#include <oto.h>

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
    int64_t end = begin;

    int64_t nest = 0;
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