#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compile.h"
#include "expr.h"
#include "../utils/iStack.h"
#include "../utils/util.h"
#include "../vm/ic.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

int32_t blockCompile(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t ppc = *pc;
    int32_t nest = 0;

    while (1) {
        int32_t tc = tcBuf->tc[ppc++];
        if (tc == TcCrBrOpn) {
            nest++;
            continue;
        } else if (tc == TcCrBrCls) {
            nest--;
            if (nest != 0) continue;
            else break;
        }
    }

    int32_t start = *pc + 1;
    int32_t end   = ppc - 2;

    compile_sub(tcBuf, var, ic, icp, start, end);

    return ppc;
}

int32_t cmpBlockCompile(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t ppc = *pc;
    int32_t nest = 0;

    while (1) {
        int32_t tc = tcBuf->tc[ppc++];
        if (tc == TcBrOpn) {
            nest++;
            continue;
        } else if (tc == TcBrCls) {
            nest--;
            if (nest != 0) continue;
            else break;
        }       
    }

    int32_t start = *pc + 1;
    int32_t end   = ppc - 2;

    expr(tcBuf, icp, &start, var, ic, end + 1);

    return ppc;    // "{"の位置を返す
}

void ifControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t ppc = *pc + 1;

    ppc = cmpBlockCompile(tcBuf, icp, &ppc, var, ic);

    // 空のJZ命令をputし、JZ命令が格納されているicpを保存する
    int32_t jmpIcp1 = *icp;
    putIc(ic, icp, OpJz, 0, 0, 0, 0);

    ppc = blockCompile(tcBuf, icp, &ppc, var, ic);

    if (tcBuf->tc[ppc] == TcElse) {
        int32_t jmpIcp2 = *icp;
        putIc(ic, icp, OpJmp, 0, 0, 0, 0);
        putIc(ic, &jmpIcp1, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);

        ppc++; // else { の分2個進める

        ppc = blockCompile(tcBuf, icp, &ppc, var, ic);

        putIc(ic, &jmpIcp2, OpJmp, (var_t *)((int64_t)*icp), 0, 0, 0);
    } else {
        putIc(ic, &jmpIcp1, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);
    }

    *pc = ppc;

    return; 
}

void whileControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t ppc = *pc + 1;

    int32_t jmpIcp1 = *icp;
    ppc = cmpBlockCompile(tcBuf, icp, &ppc, var, ic);

    int32_t jmpIcp2 = *icp;
    putIc(ic, icp, OpJz, 0, 0, 0, 0);

    ppc = blockCompile(tcBuf, icp, &ppc, var, ic);

    putIc(ic, icp, OpJmp, (var_t *)((int64_t)jmpIcp1), 0, 0, 0);
    putIc(ic, &jmpIcp2, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);

    *pc = ppc;

    return; 
}