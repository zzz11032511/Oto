#include <stdio.h>
#include <assert.h>
#include "../src/util.h"
#include "../src/token.h"
#include "../src/lexer.h"

// lexerのテスト
int main(void)
{
    tokenBuf_t *tcBuf = newTokenBuf();    // トークン番号リスト
    int var[MAX_TC];

    tcInit(tcBuf, var);

    String src = "int 1 = 0;\n 1 + 23.5671 * 4 / 1;";
    int truetc[] = {25, 29, 20, 30, 0, 29, 14, 31, 16, 32, 17, 29, 0};

    int len = lexer(src, tcBuf, var);
    int tc = 0;

    for (int i = 0; i < len; i++) {
        tc = tcBuf->tc[i];
        // assert(truetc[i] == tc);
        printf("tc : %d, ts : ", tc, tcBuf->tokens[tc]->ts);

        for (int j = 0; j < tcBuf->tokens[tc]->tl; j++) {
            printf("%c", tcBuf->tokens[tc]->ts[j]);
        }
        printf("\n");
    }
    printf("\n");
    
    return 0;
}