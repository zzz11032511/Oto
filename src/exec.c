#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"
#include "compile.h"

void exec(int **ic, int *var)
{
    int **icp = ic;

    while (1) {
        printf("opcode : %d\n", (int)icp[0]);
        switch ((int) icp[0]) {
        case OpAdd:
            printf("%d\n", (*icp[1]) + (*icp[2]));
            icp += 5;
            continue;
        case OpSub:
            printf("%d\n", (*icp[1]) - (*icp[2]));
            icp += 5;
            continue;
        case OpMul:
            printf("%d\n", (*icp[1]) * (*icp[2]));
            icp += 5;
            continue;
        case OpDiv:
            printf("%f\n", (*icp[1]) / (double)(*icp[2]));
            icp += 5;
            continue;
        default:
            return;
        }
    }
}