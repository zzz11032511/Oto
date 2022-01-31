#include <oto/oto.h>
#include <oto/oto_sound.h>

static jmp_buf env;

static Status *oto_status    = NULL;
static VectorPTR *var_list   = NULL;

static char *src = NULL;
static VectorI64 *src_tokens = NULL;
static VectorPTR *ic_list    = NULL;

void oto_init(char *srcpath) {
    oto_status = get_oto_status();
    init_option(oto_status, srcpath);

#ifdef DEBUG
    printf("srcpath : %s\n", oto_status->root_srcpath);
    printf("include : %s\n", oto_status->include_srcpath);
    printf("language : %I64d\n", oto_status->language);
    printf("timecount : %d\n", oto_status->timecount_flag);
    printf("repl : %d\n", oto_status->repl_flag);
    printf("sampling_rate : %I64d\n", oto_status->sampling_rate);
    printf("fade_range : %f\n", oto_status->fade_range);
    printf("safety : %d\n", oto_status->safety_flag);
#endif
    init_sound_stream(oto_status);

    // 変数表に予約語などを追加する
    var_list = new_vector_ptr(DEFAULT_MAX_TC);
    if (IS_NULL(var_list)) {
        exit(EXIT_FAILURE);
    }
    init_var_list(var_list);
    init_filter(var_list);
    
    // メモリ解放とPortAudio終了処理用のハンドラを登録する
    if (atexit(oto_exit) != 0) {
        exit(EXIT_FAILURE);
    }
}

void oto_exit() {
    terminate_sound_stream();
    free_vector_i64(src_tokens);
    free_vector_ptr(ic_list);
    free_vector_ptr(var_list);
    free(src);

#ifdef DEBUG
    printf("success\n");
#endif
}

void oto_error_throw(errorcode_t err) {
    longjmp(env, err);
}

void oto_error(errorcode_t err) {
    print_error(err, oto_status);
    oto_error_throw(err);
}

void oto_run() {
    if (oto_status->repl_flag && oto_status->root_srcpath == NULL) {
        repl();
        return;
    }
    
    src = src_open(oto_status->root_srcpath);
    if (IS_NULL(src)) {
        print_error(OTO_FILE_NOT_FOUND_ERROR, oto_status);
        printf("filename : %s\n", oto_status->root_srcpath);
        repl();
        return;
    }
    oto_status->repl_flag = false;

    if (setjmp(env) == 0) {
        src_tokens = new_vector_i64(DEFAULT_MAX_TC);
        if (IS_NULL(src_tokens)) {
            oto_error(OTO_INTERNAL_ERROR);
        }

        time_t start_time = 0;
        time_t end_time = 0;
        if (oto_status->timecount_flag) {
            start_time = clock();
        }

        src_tokens = lexer(src, var_list, oto_status);
#ifdef DEBUG
        print_src_tokens(src_tokens);
        print_var(var_list);
#endif

        ic_list = compile(src_tokens, var_list, src, oto_status);
#ifdef DEBUG
        print_ic_list(ic_list);
#endif

        if (oto_status->timecount_flag) {
            end_time = clock();
            if (oto_status->language == LANG_JPN_KANJI) {
                printf("コンパイル時間 : %f[秒]\n\n", CALC_TIME(start_time, end_time));
            } else if (oto_status->language == LANG_JPN_HIRAGANA) {
                printf("じゅんびじかん : %f[びょう]\n\n", CALC_TIME(start_time, end_time));
            } else if (oto_status->language == LANG_ENG) {
                printf("Compile time : %f[s]\n\n", CALC_TIME(start_time, end_time));
            }

            start_time = clock();
            exec(ic_list, var_list, oto_status);
            end_time = clock();

            if (oto_status->language == LANG_JPN_KANJI) {
                printf("実行時間 : %f[s]\n\n", CALC_TIME(start_time, end_time));
            } else if (oto_status->language == LANG_JPN_HIRAGANA) {
                printf("しゅうりょうじかん : %f[びょう]\n\n", CALC_TIME(start_time, end_time));
            } else if (oto_status->language == LANG_ENG) {
                printf("Run time : %f[s]\n\n", CALC_TIME(start_time, end_time));
            }

        } else {
            exec(ic_list, var_list, oto_status);
        }

    } else {
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}


void print_repl_help() {
    printf("\n");

    if (oto_status->language == LANG_JPN_KANJI) {
        printf("- 操作方法 -\n");
        printf("終了するときは, 「EXIT」と打つか, Ctrl+Cを押してください\n");
        printf("- 命令一覧 -\n");
        printf("PLAY  <周波数[Hz]>, <音の長さ[s]>, <音の大きさ[0-100]>, <音の種類>\n");
        printf("BEEP  <周波数[Hz]>, <音の長さ[s]>\n");
        printf("PRINT <出力したいもの>\n");
        printf("PRINTVAR\n");
        printf("PRINTWAV  <周波数[Hz]>, <音の長さ[s]>, <音の大きさ[0-100]>, <音の種類>\n");
        printf("SLEEP <長さ[秒]>\n");
    } else if (oto_status->language == LANG_JPN_HIRAGANA) {
        printf("- そうさほうほう -\n");
        printf("おわるときは, 「EXIT」とうつか, CtrlキーとCキーをどうじにおしてね\n");
        printf("- コマンドいちらん -\n");
        printf("PLAY  <おとのたかさ>, <おとのながさ[びょう]>, <おとのおおきさ[0-100]>, <おとのしゅるい>\n");
        printf("BEEP  <おとのたかさ>, <おとのながさ[びょう]>\n");
        printf("PRINT <がめんにひょうじしたいもの>\n");
        printf("PRINTVAR\n");
        printf("PRINTWAV  <おとのたかさ>, <おとのながさ[びょう]>, <おとのおおきさ[0-100]>, <おとのしゅるい>\n");
        printf("SLEEP <ながさ[びょう]>\n");
    } else if (oto_status->language == LANG_ENG) {
        printf("- Usage -\n");
        printf("If you want to exit, type \"EXIT\" or press Ctrl+C.\n");
        printf("- List of instructions -\n");
        printf("PLAY  <frequency[Hz]>, <length[s]>, <volume[0-100]>, <Sound>\n");
        printf("BEEP  <frequency[Hz]>, <length[s]>\n");
        printf("PRINT <variable or literal>\n");
        printf("PRINTVAR\n");
        printf("PRINTWAV  <frequency[Hz]>, <length[s]>, <volume[0-100]>, <Sound>\n");
        printf("SLEEP <length[s]>\n");
    }

    printf("\n");
}

void print_repl_guide() {
    printf("\nOto (beta 2021-12-04) REPL ... Hello!!!\n");

    if (oto_status->language == LANG_JPN_KANJI) {
        printf("コマンド一覧を見たいときは「HELP」\n");
        printf("終了するときは「EXIT」\n");
    } else if (oto_status->language == LANG_JPN_HIRAGANA) {
        printf("コマンドいちらんをみたいときは「HELP」\n");
        printf("おわるときは「EXIT」\n");
        printf("をにゅうりょくしてください\n");
    } else if (oto_status->language == LANG_ENG) {
        printf("\"HELP\" ... See list of instructions\n");
        printf("\"EXIT\" ... Exit\n");
    }
}

void print_repl_exit() {
    if (oto_status->language == LANG_JPN_KANJI) {
        printf("REPLモードを終了します\n");
    } else if (oto_status->language == LANG_JPN_HIRAGANA) {
        printf("REPLモードをおわります\n");
    } else if (oto_status->language == LANG_ENG) {
        printf("Exit REPL.\n");
    }
}

#define REPL_STR_BUFSIZE 10000
void repl() {
    oto_status->repl_flag = true;
    print_repl_guide();

    char str[REPL_STR_BUFSIZE] = {0};

    while (true) {
        src_tokens = NULL;
        ic_list = NULL;

        printf(">>> ");
        fgets(str, REPL_STR_BUFSIZE, stdin);
        
        int i = strlen(str);
        if (str[i - 1] == '\n') {
            str[i - 1] = 0;
        }
        
        if (setjmp(env) == 0) {
            src_tokens = new_vector_i64(DEFAULT_MAX_TC);
            if (IS_NULL(src_tokens)) {
                oto_error(OTO_INTERNAL_ERROR);
            }

            if (strncmp_cs(str, "EXIT", 4) == 0) {
                print_repl_exit();
                break;
            } else if (strncmp_cs(str, "HELP", 4) == 0) {
                print_repl_help();
            }

            src_tokens = lexer(str, var_list, oto_status);

            ic_list = compile(src_tokens, var_list, str, oto_status);
            
            exec(ic_list, var_list, oto_status);
        }

        free_vector_i64(src_tokens);
        free_vector_ptr(ic_list);
    }
}
