#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "errorHandle.h"
#include "vm/ic.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "variable/variable.h"
#include "utils/util.h"

void printVarName(tokenBuf_t *tcBuf, int32_t tc) {
    for (int i = 0; i < tcBuf->tokens[tc]->tl; i++) {
        printf("%c", tcBuf->tokens[tc]->ts[i]);
    }
}

static const str_t opName[] = {
    "Nop",
    "CpyD",
    "CpyP",
    "Push",
    "Add",
    "Sub",
    "Mul",
    "Div",
    "Mod",
    "And",
    "Or",
    "Add1",
    "Sub1",
    "Eq",
    "NEq",
    "LtCmp",
    "LtEqCmp",
    "RiCmp",
    "RiEqCmp",
    "Loop",
    "Jmp",
    "Jz",
    "Jnz",
    "Print",
    "Exit",
};

void printIc(var_t **icp, tokenBuf_t *tcBuf) {
    var_t **p = icp;
    uint64_t icNum = 0;

    while (1) {
        uint64_t op = (uint64_t)p[0];

        // printf(
        //     "%5ld : %9s, v1 : %16lx, v2 : %16lx, v3 : %16lx, v4 : %16lx\n",
        //     icNum, opName[op], (uint64_t)p[1], (uint64_t)p[2], (uint64_t)p[3], (uint64_t)p[4]
        // );

        printf("%5ld : %9s    ", icNum, opName[op]);

        for (int i = 1; i <= 4; i++) {
            if ((OpJmp <= op && op <= OpJnz) && i == 1) {
                printf("%lu", (int64_t)p[i]);
            } else if (p[i] == 0) {
                break;
            } else {
                printVarName(tcBuf, p[i]->tc);
            }
            printf("    ");
        }
        printf("\n");
 
        if (op == OpExit) {
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
        if (tcNum == TcLF) {
            printf("tc[%d] : \\n", i);
        } else {
            printf("tc[%d] : ", i);
            printVarName(tcBuf, tcNum);
        }
        printf("\n");
    }
    printf("\n");
}

void printTokenList(tokenBuf_t *tcBuf) {
    for (int i = 0; i < tcBuf->tcs; i++) {
        int32_t tcNum = tcBuf->tokens[i]->tc;
        if (tcNum == TcLF) {
            printf("tc[%d] : \\n", i);
        } else {
            printf("tc[%d] : ", i);
            printVarName(tcBuf, tcNum);
        }
        printf("\n");
    }
    printf("\n");
}

void printRpnTc(tokenBuf_t *tcBuf, int32_t *rpnTc, int32_t length) {
    printf("rpn : ");
    for (int i = 0; i < length; i++) {
        int32_t tcNum = rpnTc[i];
        printVarName(tcBuf, tcNum);      
    }
    printf("\n");
}
