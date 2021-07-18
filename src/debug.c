#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "vm/ic.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "variable/variable.h"
#include "utils/util.h"

static const str_t opName[] = {
    "Nop",
    "Def",
    "CpyS",
    "CpyP",
    "Push",
    "Add",
    "Sub",
    "Mul",
    "Div",
    "Mod",
    "Add1",
    "Sub1",
    "And",
    "Or",
    "Eq",
    "NEq",
    "LtCmp",
    "LtEqCmp",
    "RiCmp",
    "RiEqCmp",
    "Jmp",
    "Jz",
    "Jnz",
    "Print",
    "End",
};

void printIc(var_t **icp) {
    var_t **p = icp;
    uint64_t icNum = 0;

    while (1) {
        uint64_t op = (uint64_t)p[0];
        printf(
            "%5ld : %9s, v1 : %16ld, v2 : %16ld, v3 : %16ld, v4 : %16ld\n",
            icNum, opName[op], (uint64_t)p[1], (uint64_t)p[2], (uint64_t)p[3], (uint64_t)p[4]
        );
        if (op == OpEnd) {
            break;
        }
        icNum += 5;
        p += 5;
    }
    printf("\n");
}

void printTokenCode(tokenBuf_t *tcBuf, int32_t length) {
    for (int i = 0; i < length; i++) {
        int32_t tcNum = tcBuf->tc[i];
        printf("tc[%d] : ", i);

        // tokenの長さの分だけ1文字ずつ出力する
        for (int j = 0; j < tcBuf->tokens[tcNum]->tl; j++)
            printf("%c", tcBuf->tokens[tcNum]->ts[j]);
            
        printf("\n");
    }
    printf("\n");
}

void printRpnTc(tokenBuf_t *tcBuf, int32_t *rpnTc, int32_t length) {
    printf("rpn : ");
    for (int i = 0; i < length; i++) {
        int32_t tcNum = rpnTc[i];

        // tokenの長さの分だけ1文字ずつ出力する
        for (int j = 0; j < tcBuf->tokens[tcNum]->tl; j++)
            printf("%c", tcBuf->tokens[tcNum]->ts[j]);        
    }
    printf("\n");
}
