#include <oto.h>

/* idxは最初の「"」の位置 */
char *new_string_literal(char *src, int64_t idx) {
    if (src[idx] != '"') {
        return NULL;
    }
    idx++;

    size_t len = 0;
    while (src[idx + len] != '"') {
        len++;
        if (src[idx + len] == '\0') {
            return NULL;
        }
    }

    char *str = MYMALLOC(len + 1, char);
    if (IS_NULL(str)) {
        return NULL;
    }
    
    strncpy(str, &src[idx], len);

    return str;
}

static Map *include_file_list = NULL;
/**
 * 循環参照発見機構の初期化
 * 
 * 引数には実行した最初のファイルのパスを渡す
 */
void init_include_file_manager(char *root_path) {
    if (include_file_list == NULL) {
        include_file_list = new_map();
        if (IS_NULL(include_file_list)) {
            return;
        }
    }
    map_puti(include_file_list, root_path, 1);
}

/**
 * 循環参照を発見する
 * 
 * インクルードしたファイルのパスを
 * include_file_listに参照回数1回として追加し
 * もし既に参照されていた場合はエラーとする
 */
static void check_circular_ref(char *path) {
    if (map_exist_key(include_file_list, path)) {
        if (map_geti(include_file_list, path) >= 1) {
            // 循環参照
            printf("Circular reference!\n");
            exit(1);
        } else {
            map_inc_val(include_file_list, path);
        }
    } else {
        map_puti(include_file_list, path, 1);
    }
}

static void include_file(char *src, int64_t idx, VectorI64 *src_tokens) {
    // "include"の分
    idx += 7;

    while (src[idx] == ' ') {
        idx++;
        if (src[idx] == '\n' && src[idx] == '\0') {
            // pathが指定されていない
            printf("include error1\n");
            return;
        }
    }

    char *path = NULL;
    path = new_string_literal(src, idx);
    if (IS_NULL(path)) {
        printf("include error2\n");
        return;
    }
    check_circular_ref(path);

    char *new_src = src_open(path);
    if (IS_NULL(new_src)) {
        printf("include error4\n");
        return;
    }

    tokenize(new_src, src_tokens);

    // ファイル参照回数を1減らす
    map_dec_val(include_file_list, path);

    free(new_src);
    free(path);
}

void preprocess(char *src, int64_t idx, VectorI64 *src_tokens) {
    idx++;

    if (strncmp_cs("include", &src[idx], 7) == 0) {
        include_file(src, idx, src_tokens);
    }
}
