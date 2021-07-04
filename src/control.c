#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compile.h"
#include "iStack.h"
#include "ic.h"
#include "expr.h"
#include "lexer.h"
#include "token.h"
#include "util.h"
#include "variable.h"

void ifControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t cmpStartPc = *pc + 1;
    int32_t cmpEndPc = 0;

    int32_t ppc = cmpStartPc;
    int32_t nest = 0;
    while (1) {
        int32_t tc = tcBuf->tc[ppc++];
        if (tc == TcBrOpn) {
            nest++;
            continue;

        } else if (tc == TcBrCls) {
            nest--;
            if (nest != 0) {
                continue;
            } else {
                break;
            }
        }       
    }
    cmpEndPc = ppc - 1;

    printf("cmpStartPc : %d, cmpEndPc : %d\n", cmpStartPc, cmpEndPc);
    expr(tcBuf, icp, &cmpStartPc, var, ic, cmpEndPc + 1);
    *pc = cmpStartPc;
    
    // ここの地点ではtrueだったとき実行する箇所の先頭のpcが入ってるはず
    ppc = *pc - 1;
    printf("ppc : %d\n", ppc);

    int32_t start = *pc;

    // if文の終わりを探す
    nest = 0;
    while (1) {
        int32_t tc = tcBuf->tc[ppc++];
        if (tc == TcCrBrOpn) {
            nest++;
            continue;

        } else if (tc == TcCrBrCls) {
            nest--;
            if (nest != 0) {
                continue;
            } else {
                break;
            }
        }
    }

    int32_t end = ppc - 2;
    printf("start : %d, end : %d\n", start, end);

    // 空のJZ命令をputし、JZ命令が格納されているicpを保存する
    int32_t jmpIcp = *icp;
    putIc(ic, icp, OpJz, 0, 0, 0, 0);

    compile_sub(tcBuf, var, ic, icp, start, end);

    putIc(ic, &jmpIcp, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);

    *pc = ppc;

    return; 
}