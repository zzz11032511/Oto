#include <oto.h>

static bool timecount_flag = false;
void set_timecount_flag(bool flag) {
    timecount_flag = flag;
}

void oto_init(char *path) {
    if (atexit(oto_quit) != 0) {
        printf("Unknown error :(\n");
        exit(EXIT_FAILURE);
    }

    init_token_list();
    init_rsvword();
    init_include_file_manager(path);
}

static char *src = NULL;
static VectorI64 *src_tokens = NULL;
static VectorPTR *var_list   = NULL;
static VectorPTR *ic_list    = NULL;

void oto_run(const char *path) {
    src = src_open(path);
    if (IS_NULL(src)) {
        printf("error\n");
        return;
    }

    src_tokens = lexer(src);
    if (IS_NULL(src_tokens)) {
        printf("error\n");
        return;
    }

    var_list = make_var_list();
    if (IS_NULL(var_list)) {
        printf("error\n");
        return;
    }

    ic_list = compile(src_tokens, var_list);
    if (IS_NULL(ic_list)) {
        printf("error\n");
        return;
    }

#ifdef DEBUG
    print_src_tokens(src_tokens);
    print_var(var_list);
    print_ic_list(ic_list);
#endif

    exec(ic_list);
}

void oto_quit() {
    free_vector_i64(src_tokens);
    free_vector_ptr(ic_list);
    free_var_list(var_list);
    free_token_list();
    free(src);

    printf("success\n");
}
