#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#include "error/error.h"
#include "debug/debug.h"
#include "util/util.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "vm/exec.h"

static bool timecount_flag = false;
void set_timecount_flag(bool f) {
    timecount_flag = f;
}

void run(const int8_t *filename) {
    int8_t *src = NULL;

    src = src_open((char *)filename);
    if (src == NULL) {
        call_error(FILE_NOT_FOUND_ERROR);
        return;
    }
    set_src(src);
    
    time_t start_time = 0;
    time_t end_time = 0;

    if (timecount_flag) {
        start_time = clock();
    }

    lexer(src);
#ifdef DEBUG
    print_all_token();
    print_converted_source();
#endif
    parser();

    if (timecount_flag) {
        end_time = clock();
        printf("compile time : %f[s]\n\n", CALC_TIME(start_time, end_time));
    }

#ifdef DEBUG
    print_opcodes();
#endif

    if (timecount_flag) {
        start_time = clock();
        exec();
        end_time = clock();
        printf("execute time : %f[s]\n\n", CALC_TIME(start_time, end_time));
        return;
    }

    exec();
}