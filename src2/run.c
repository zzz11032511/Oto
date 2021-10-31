#include "oto.h"

static bool timecount_flag = false;
void set_timecount_flag(bool flag) {
    timecount_flag = flag;
}

void oto_init() {
    init_token_list();
}

void oto_run(const int8_t *path) {
    char *src = src_open(path);
    if (IS_NULL(src)) {
        printf("error");
        return;
    }

    VectorUI64 *src_tokens = NULL;
    VectorPTR *var_list = NULL;
    lexer(src, src_tokens, var_list);
}

void oto_quit() {
}