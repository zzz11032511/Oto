#include <stdio.h>
#include <time.h>
#include "compile.h"

// ic[]に格納された内部コードを高速に実行する
void exec(int *ic[])
{
    clock_t t0 = clock();
    int **icp = ic;
    int i;
    while (1) {
        switch ((int) icp[0]) {
        case OpNeg:     *icp[1] = - *icp[2];          icp += 5; continue;
        case OpAdd1:    (*icp[1])++;                  icp += 5; continue;
        case OpMul:     *icp[1] = *icp[2] *  *icp[3]; icp += 5; continue;
        case OpDiv:     *icp[1] = *icp[2] /  *icp[3]; icp += 5; continue;
        case OpMod:     *icp[1] = *icp[2] %  *icp[3]; icp += 5; continue;
        case OpAdd:     *icp[1] = *icp[2] +  *icp[3]; icp += 5; continue;
        case OpSub:     *icp[1] = *icp[2] -  *icp[3]; icp += 5; continue;
        case OpClt:     *icp[1] = *icp[2] <  *icp[3]; icp += 5; continue;
        case OpCge:     *icp[1] = *icp[2] >= *icp[3]; icp += 5; continue;
        case OpCle:     *icp[1] = *icp[2] <= *icp[3]; icp += 5; continue;
        case OpCgt:     *icp[1] = *icp[2] >  *icp[3]; icp += 5; continue;
        case OpCeq:     *icp[1] = *icp[2] == *icp[3]; icp += 5; continue;
        case OpCne:     *icp[1] = *icp[2] != *icp[3]; icp += 5; continue;
        case OpAnd:     *icp[1] = *icp[2] &  *icp[3]; icp += 5; continue;
        case OpCpy:     *icp[1] = *icp[2];            icp += 5; continue;
        case OpPrint:
            printf("%d\n", *icp[1]);
            icp += 5;
            continue;
        case OpGoto:                           icp = (int **) icp[1]; continue;
        case OpJeq:  if (*icp[2] == *icp[3]) { icp = (int **) icp[1]; continue; } icp += 5; continue;
        case OpJne:  if (*icp[2] != *icp[3]) { icp = (int **) icp[1]; continue; } icp += 5; continue;
        case OpJlt:  if (*icp[2] <  *icp[3]) { icp = (int **) icp[1]; continue; } icp += 5; continue;
        case OpTime:
            printf("time: %.3f[sec]\n", (clock() - t0) / (double) CLOCKS_PER_SEC);
            icp += 5;
            continue;
        case OpEnd:
            return;
        case OpLop:
            i = *icp[2];
            i++;
            *icp[2] = i;
            if (i < *icp[3]) {
                icp = (int **) icp[1];
                continue;
            }
            icp += 5;
            continue;
        } 
    }
}