#include <oto.h>
#include <oto_sound.h>

static Status *oto_status = NULL;

static char *src = NULL;
static VectorI64 *src_tokens = NULL;
static VectorPTR *var_list   = NULL;
static VectorPTR *ic_list    = NULL;

void oto_init(char *srcpath) {
    oto_status = get_oto_status();
    init_status(oto_status, srcpath);
    
    init_token_list();
    init_rsvword();
    // init_include_file_manager(path);
    init_sound_stream(oto_status->sampling_rate);
    
    if (atexit(oto_exit_process) != 0) {
        exit(EXIT_FAILURE);
    }
}

void oto_run() {
    src = src_open(oto_status->root_srcpath);
    if (IS_NULL(src)) {
        // print_error(OTO_FILE_NOT_FOUND_ERROR);
        exit(EXIT_FAILURE);
    }

    time_t start_time = 0;
    time_t end_time = 0;
    if (oto_status->timecount_flag) {
        start_time = clock();
    }

    lexer(src, src_tokens, var_list, oto_status);
#ifdef DEBUG
    print_src_tokens(src_tokens);
#endif

    var_list = make_var_list();
#ifdef DEBUG
    print_var(var_list);
#endif

    ic_list = compile(src_tokens, var_list, src);
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
        exec(ic_list, var_list);
        end_time = clock();

        if (oto_status->language == LANG_JPN_KANJI) {
            printf("実行時間 : %f[s]\n\n", CALC_TIME(start_time, end_time));
        } else if (oto_status->language == LANG_JPN_HIRAGANA) {
            printf("しゅうりょうじかん : %f[びょう]\n\n", CALC_TIME(start_time, end_time));
        } else if (oto_status->language == LANG_ENG) {
            printf("Run time : %f[s]\n\n", CALC_TIME(start_time, end_time));
        }

    } else {
        exec(ic_list, var_list);
    }
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

void print_help() {
    printf("\n");

    if (oto_status->language == LANG_JPN_KANJI) {
        printf("- 操作方法 -\n");
        printf("終了するときは, 「EXIT」と打つか, Ctrl+Cを押してください\n");
        printf("- 命令一覧 -\n");
        printf("PLAY  <周波数>, <音の長さ>, <音の大きさ>, <音の種類>\n");
        printf("BEEP  <周波数>, <音の長さ>\n");
        printf("PRINT <出力したいもの>\n");
    } else if (oto_status->language == LANG_JPN_HIRAGANA) {
        printf("- そうさほうほう -\n");
        printf("おわるときは, 「EXIT」とうつか, CtrlキーとCキーをどうじにおしてね\n");
        printf("- コマンドいちらん -\n");
        printf("PLAY  <おとのたかさ>, <おとのながさ>, <おとのおおきさ>, <おとのしゅるい>\n");
        printf("BEEP  <おとのたかさ>, <おとのながさ>\n");
        printf("PRINT <がめんにひょうじしたいもの>\n");
    } else if (oto_status->language == LANG_ENG) {
        printf("- Usage -\n");
        printf("If you want to finish, type \"EXIT\" or press Ctrl+C.\n");
        printf("- List of instructions -\n");
        printf("PLAY  <frequency>, <length>, <volume>, <Sound>\n");
        printf("BEEP  <frequency>, <length>\n");
        printf("PRINT <variable or literal>\n");
    }

    printf("\n");
}

#define REPL_STR_BUFSIZE 10000
void repl() {
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

    char str[REPL_STR_BUFSIZE] = {0};

    var_list = make_var_list();
    while (true) {
        // エラーのときにここに戻れるようにしたいけど...
        printf(">>> ");
        fgets(str, REPL_STR_BUFSIZE, stdin);
        
        int i = strlen(str);
        if (str[i - 1] == '\n') {
            str[i - 1] = 0;
        }

        if (strncmp_cs(str, "EXIT", 4) == 0) {
            if (oto_status->language == LANG_JPN_KANJI) {
                printf("REPLモードを終了します\n");
            } else if (oto_status->language == LANG_JPN_HIRAGANA) {
                printf("REPLモードをおわります\n");
            } else if (oto_status->language == LANG_ENG) {
                printf("Exit REPL.\n");
            }
            break;
        } else if (strncmp_cs(str, "HELP", 4) == 0) {
            print_help();
        }

        lexer(str, src_tokens, var_list, oto_status);

        ic_list = compile(src_tokens, var_list, str);
        
        exec(ic_list, var_list);

        free_vector_i64(src_tokens);
        free_vector_ptr(ic_list);
        src_tokens = NULL;
        ic_list = NULL;
    }
}
