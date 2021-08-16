#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "oto.h"
#include "error/error.h"
#include "parser/compile.h"
#include "vm/exec.h"
#include "lexer/token.h"
#include "variable/variable.h"
#include "utils/util.h"

static bool_t timecount_flag = false;
void set_timecount_flag(bool_t f) {
    timecount_flag = f;
}

void run(const str_t path) {
    str_t src = NULL;
    uint32_t size = 0;

    if ((size = src_load(path, &src)) == -1) {
        oto_quit(EXIT_FAILURE);
    }

    tokenbuf_t *tcbuf = new_tokenbuf(); // トークン情報
    
    var_t var_list[MAX_TC];
    init_var_list(var_list, MAX_TC);

    var_t *ic[IC_LIST_SIZE];

    set_error_all(path, src, tcbuf, var_list, ic);

    if (timecount_flag) {
        time_t start_time, end_time;

        start_time = clock();
        compile(src, size, tcbuf, var_list, ic);
        end_time = clock();

        printf("compile time : %f[s]\n\n", CALC_TIME(start_time, end_time));

        start_time = clock();
        exec(ic, var_list, tcbuf);
        end_time = clock();

        printf("\nexec time : %f[s]\n", CALC_TIME(start_time, end_time));
        return;
    }

    compile(src, size, tcbuf, var_list, ic);
    exec(ic, var_list, tcbuf);
}