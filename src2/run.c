#include <oto.h>

static bool timecount_flag = false;
void set_timecount_flag(bool flag) {
    timecount_flag = flag;
}

void oto_init(char *path) {
    if (atexit(oto_quit) != 0) {
        print_error(OTO_UNKNOWN_ERROR);
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

    src_tokens = lexer(src);
    var_list = make_var_list();
    ic_list = compile(src_tokens, var_list);

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

#ifdef DEBUG
    printf("success\n");
#endif
}
