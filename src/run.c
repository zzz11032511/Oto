#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser/compile.h"
#include "vm/exec.h"
#include "vm/ic.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "utils/util.h"

int32_t run(str_t src) {
    tokenBuf_t *tcBuf = newTokenBuf();  // トークン番号リスト
    var_t var[MAX_TC];

    tcInit(tcBuf, var);

    var_t *ic[IC_LIST_SIZE];
    if (compile(src, tcBuf, var, ic) >= 1) {
        exit(1);
    }
    exec(ic, var);

    freeTokenBuf(tcBuf);
    return 0;
}
