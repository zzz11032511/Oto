#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "lexer.h"
#include "compile.h"
#include "ic.h"
#include "token.h"
#include "exec.h"

int run(String src) {
    tokenBuf_t *tcBuf = newTokenBuf();    // トークン番号リスト
    int var[MAX_TC];

    // TODO: symbolsが0終端じゃないからエラー吐く
    tcInit(tcBuf, var);

    int *ic[IC_LIST_SIZE];
    if (compile(src, tcBuf, var, ic) >= 1) {
        exit(1);
    }
    exec(ic, var);

    freeTokenBuf(tcBuf);
    return 0;
}
