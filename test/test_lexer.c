#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "../src/util.h"
#include "../src/token.h"
#include "../src/lexer.h"

// lexerのテスト
int main(void)
{
    tokenBuf_t *tcBuf = newTokenBuf();    // トークン番号リスト
    int32_t var[MAX_TC];

    tcInit(tcBuf, var);

    str_t src = "int 1 = 0;\n 1 + 23.5671 * 4 / 1;";
    int32_t truetc[] = {25, 29, 20, 30, 0, 29, 14, 31, 16, 32, 17, 29, 0};

    int32_t len = lexer(src, tcBuf, var);
    int32_t tc = 0;

    for (uint32_t i = 0; i < len; i++) {
        tc = tcBuf->tc[i];
        // assert(truetc[i] == tc);
        printf("tc : %d, ts : ", tc, tcBuf->tokens[tc]->ts);

        for (uint32_t j = 0; j < tcBuf->tokens[tc]->tl; j++) {
            printf("%c", tcBuf->tokens[tc]->ts[j]);
        }
        printf("\n");
    }
    printf("\n");
    
    return 0;
}