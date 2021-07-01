#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "compile.h"
#include "exec.h"
#include "ic.h"
#include "lexer.h"
#include "token.h"
#include "util.h"

int32_t run(str_t src) {
    tokenBuf_t *tcBuf = newTokenBuf();  // トークン番号リスト
    var_t var[MAX_TC];

    // TODO: symbolsが0終端じゃないからエラー吐く
    tcInit(tcBuf, var);

    var_t *ic[IC_LIST_SIZE];
    if (compile(src, tcBuf, var, ic) >= 1) {
        exit(1);
    }
    exec(ic, var);

    freeTokenBuf(tcBuf);
    return 0;
}
