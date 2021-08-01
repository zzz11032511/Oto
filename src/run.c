#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "errorHandle.h"
#include "parser/compile.h"
#include "vm/exec.h"
#include "lexer/token.h"
#include "utils/util.h"

#define IC_LIST_SIZE 20000    // 内部コード列の長さ

int32_t run(str_t src) {
    tokenBuf_t *tcBuf = newTokenBuf();  // トークン情報
    var_t var[MAX_TC];
    var_t *ic[IC_LIST_SIZE];

    setTcBuf(tcBuf);
    setVar(var);
    setIc(ic);

#ifdef TIME
    time_t startTime, endTime;
    startTime = clock();
#endif

    compile(src, tcBuf, var, ic);

#ifdef TIME
    endTime = clock();
    printf("compile time : %f[s]\n", CALC_TIME(startTime, endTime));
#endif

#ifdef TIME
    startTime = clock();
#endif
    exec(ic, var, tcBuf);
#ifdef TIME
    endTime = clock();
    printf("exec time : %f[s]\n", CALC_TIME(startTime, endTime));
#endif

    return 0;
}