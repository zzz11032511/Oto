#include <stdio.h>
#include "util.h"
#include "lexer.h"
#include "compile.h"
#include "token.h"
#include "variable.h"

int run(String src) {
    tokenBuf_t *tcBuf = newTokenBuf();    // トークン番号リスト
    var_t *var[MAX_TC + 1];               // 変数リスト
    
    // TODO: symbolsが0終端じゃないからエラー吐く
    tcInit(&tcBuf);
    lexer(src, &tcBuf);

    if (compile(src, tcBuf, var) >= 1) {
        fprintf(stderr, "compile error\n");
        exit(1);
    }
    freeTokenBuf(&tcBuf);
    return 0;
}
