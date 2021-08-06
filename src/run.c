#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "oto.h"
#include "error/error.h"
#include "parser/compile.h"
#include "vm/exec.h"
#include "lexer/token.h"
#include "utils/util.h"

int32_t run(const str_t path) {
    str_t src = NULL;

    if (src_load(path, &src)) {
        return 1;
    }

    tokenbuf_t *tcbuf = new_tokenbuf(); // トークン情報
    var_t var_list[MAX_TC];
    var_t *ic[IC_LIST_SIZE];

    set_error_all(path, src, tcbuf, var_list, ic);

#ifdef TIME
    time_t start_time, end_time;
    start_time = clock();
#endif
    compile(src, tcbuf, var_list, ic);
#ifdef TIME
    end_time = clock();
    printf("compile time : %f[s]\n", CALC_TIME(start_time, end_time));
#endif

#ifdef TIME
    start_time = clock();
#endif
    exec(ic, var_list, tcbuf);
#ifdef TIME
    end_time = clock();
    printf("exec time : %f[s]\n", CALC_TIME(start_time, end_time));
#endif

    return 0;
}