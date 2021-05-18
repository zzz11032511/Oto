#include <stdio.h>
#include "util.h"
#include "lexer.h"
#include "compile.h"
#include "token.h"

int run(String src) {
    tokenBuf_t *tcBuf = newTokenBuf();
    
    // TODO: symbolsが0終端じゃないからエラー吐く
    // tcInit(tcBuf);
    lexer(src, tcBuf);

    freeTokenBuf(tcBuf);
    return 0;
}
