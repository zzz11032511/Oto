#include <oto/oto.h>

/**
 * 循環参照を発見する
 * 
 * インクルードしたファイルのパスを
 * status->srcfile_tableに参照回数1回として追加し
 * もし既に参照されていた場合はエラーとする
 */
static void check_circular_ref(char *path, Status *status) {
    if (map_exist_key(status->srcfile_table, path)) {
        if (map_geti(status->srcfile_table, path) >= 1) {
            // 循環参照
            oto_error(OTO_CIRCULAR_REFERENCE_ERROR);
        } else {
            map_inc_val(status->srcfile_table, path);
        }
    } else {
        map_puti(status->srcfile_table, path, 1);
    }
}

static void include_file(char *src, int64_t idx, VectorI64 *src_tokens, VectorPTR *var_list, Status *status) {
    // "include"の分
    idx += 7;

    while (src[idx] == ' ') {
        idx++;
        if (src[idx] == '\n' && src[idx] == '\0') {
            // pathが指定されていない
            oto_error(OTO_PREPROCESS_ERROR);
        }
    }

    char *path = NULL;
    path = new_string_literal(src, idx);
    if (IS_NULL(path)) {
        oto_error(OTO_PREPROCESS_ERROR);
    }
    check_circular_ref(path, status);

    char *new_src = src_open(path);
    if (IS_NULL(new_src)) {
        oto_error(OTO_INCLUDE_FILE_NOT_FOUND_ERROR);
    }

    tokenize(new_src, src_tokens, var_list, status);

    // ファイル参照回数を1減らす
    map_dec_val(status->srcfile_table, path);

    free(new_src);
    free(path);
}

void preprocess(char *src, int64_t idx, VectorI64 *src_tokens, VectorPTR *var_list, Status *status) {
    idx++;

    if (strncmp_cs("include", &src[idx], 7) == 0) {
        include_file(src, idx, src_tokens, var_list, status);
    }
}
