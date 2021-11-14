#include <oto.h>
#include <oto_sound.h>

static char *src = NULL;
static VectorI64 *src_tokens = NULL;
static VectorPTR *var_list   = NULL;
static VectorPTR *ic_list    = NULL;

static bool timecount_flag = false;
void set_timecount_flag(bool flag) {
    timecount_flag = flag;
}

void oto_init(char *path) {
    if (atexit(oto_exit_process) != 0) {
        exit(EXIT_FAILURE);
    }

    init_token_list();
    init_rsvword();
    init_include_file_manager(path);
    init_sound_stream();
}

void oto_run(const char *path) {
    src = src_open(path);
    if (IS_NULL(src)) {
        print_error(OTO_FILE_NOT_FOUND_ERROR);
        exit(EXIT_FAILURE);
    }

    src_tokens = lexer(src);
#ifdef DEBUG
    print_src_tokens(src_tokens);
#endif

    var_list = make_var_list();
#ifdef DEBUG
    print_var(var_list);
#endif

    ic_list = compile(src_tokens, var_list);
#ifdef DEBUG
    print_ic_list(ic_list);
#endif

    exec(ic_list);
}

void oto_exit_process() {
    terminate_sound_stream();
    free_vector_i64(src_tokens);
    free_vector_ptr(ic_list);
    free_var_list(var_list);
    free_token_list();
    free(src);

#ifdef DEBUG
    printf("success\n");
#endif
}

void oto_error_exit(errorcode_t err) {
    print_error(err);
    exit(EXIT_FAILURE);
}
