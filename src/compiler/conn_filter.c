#include "compiler.h"

static int64_t get_init_filtercode(tokencode_t tc) {
    for (int64_t i = 0; i < FILTER_NUM; i++) {
        if (strcmp(def_filters[i].s, VAR(tc)->token->str) == 0) {
            return i;
        }
    }
    return -1;
}

void compile_conn_filter(int64_t *icp, SliceI64 *conntcs) {
    tokencode_t s_sound_tc = conntcs->data[0];
    tokencode_t e_sound_tc = conntcs->data[conntcs->length - 1];

    if (!IS_AVAILABLE_VAR(s_sound_tc)) {
        error_compiler(OTO_NAME_ERROR, conntcs, 0);
    } else if (!IS_AVAILABLE_VAR(e_sound_tc)) {
        error_compiler(OTO_NAME_ERROR, conntcs, conntcs->length - 1);
    }

    if (s_sound_tc != e_sound_tc) {
        put_opcode(icp, OP_CPYS, VAR(e_sound_tc), VAR(s_sound_tc), 0, 0);
    }

    for (int64_t i = 1; i < conntcs->length - 2;) {
        tokencode_t tc = conntcs->data[i];

        if (tc == TC_RARROW) {
            i++;
            int64_t fc = get_init_filtercode(conntcs->data[i]);
            if (fc == -1) {
                error_compiler(OTO_FILTER_ERROR, conntcs, i);
            }
            int64_t param = def_filters[fc].param;

            i++;
            SliceI64 *argtcs = make_args_enclosed_br(conntcs, i);

            compile_args(icp, argtcs, param);
            i += argtcs->length + 2;

            put_opcode(icp, OP_CONNFILTER, VAR(e_sound_tc), (Var *)fc, 0, 0);

        } else {
            error_compiler(OTO_INVALID_SYNTAX_ERROR, conntcs, i);
        }
    }
}