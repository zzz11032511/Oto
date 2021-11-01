#include "oto.h"

static bool timecount_flag = false;
void set_timecount_flag(bool flag) {
    timecount_flag = flag;
}

void oto_init() {
    init_token_list();
    init_rsvword();
}

void oto_run(const int8_t *path) {
    char *src = src_open(path);
    if (IS_NULL(src)) {
        printf("error\n");
        return;
    }

    VectorUI64 *src_tokens = lexer(src);
    if (IS_NULL(src_tokens)) {
        printf("error\n");
        return;
    }

    // トークン化したソースコードの表示
    printf("tc : ");
    for (uint64_t i = 0; i < src_tokens->length; i++) {
        printf("%I64d ", src_tokens->data[i]);
    }
    printf("\n\n");

    VectorPTR *var_list = make_var_list();
    if (IS_NULL(var_list)) {
        printf("error\n");
        return;
    }

    print_var(var_list);

    VectorPTR *ic_list = compile(src_tokens, var_list);
    if (IS_NULL(ic_list)) {
        printf("error\n");
        return;
    }

    exec(ic_list);

    free_vector_ui64(src_tokens);
    free_vector_ptr(ic_list);
    free_var_list(var_list);
    free_token_list();
}

void oto_quit() {

}