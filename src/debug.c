#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser/compile.h"
#include "vm/exec.h"
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

void printIc(var_t **ic, int32_t *icp, int32_t op, var_t *v1, var_t *v2, var_t *v3, var_t *v4) {
    printf(
        "pc : %5d, Opcode : %9s, v1 : %16ld, v2 : %16ld, v3 : %16ld, v4 : %16ld\n",
        *icp, opName[op], (uint64_t)v1, (uint64_t)v2, (uint64_t)v3, (uint64_t)v4
    );
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