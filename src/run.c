#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "errorHandle.h"
#include "parser/compile.h"
#include "vm/exec.h"
#include "vm/ic.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/util.h"

int32_t run(str_t src) {
    tokenBuf_t *tcBuf = newTokenBuf();  // トークン情報
    var_t var[MAX_TC];
    var_t *ic[IC_LIST_SIZE];

    setTcBuf(tcBuf);
    setVar(var);
    setIc(ic);

    compile(src, tcBuf, var, ic);

    exec(ic, var);

    freeTokenBuf(tcBuf);
    return 0;
}