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

/* "{"と"}"で囲まれたブロックの最後のトークン位置を返す */
int32_t searchBlockEnd(tokenBuf_t *tcBuf, int32_t pc) {
    int32_t nest = 0;

    while (1) {
        int32_t tc = tcBuf->tc[pc++];
        if (tc == TcCrBrOpn || tc == TcBrOpn) {
            nest++;
            continue;
        } else if (tc == TcCrBrCls || tc == TcBrCls) {
            nest--;
            if (nest != 0) continue;
            else break;
        }
    }

    return pc - 2;
}

/* "{}"で囲まれたブロック内のコードをコンパイルし、ブロックを抜けた場所のpcを返す */
int32_t blockCompile(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t start = *pc + 1;
    int32_t end   = searchBlockEnd(tcBuf, *pc);
    compile_sub(tcBuf, var, ic, icp, start, end);
    return end + 2;    // ブロックの外に持っていくため2足す
}

/* "()"で囲まれた比較式をコンパイルし、その後ろのpcを返す */
int32_t cmpBlockCompile(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t start = *pc + 1;
    int32_t end   = searchBlockEnd(tcBuf, *pc);
    expr(tcBuf, icp, &start, var, ic, end);
    return end + 2;
}

void ifControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t ppc = *pc + 1;

    ppc = cmpBlockCompile(tcBuf, icp, &ppc, var, ic);

    // 空のJZ命令をputし、JZ命令が格納されているicpを保存する
    int32_t jmpIcp1 = *icp;
    putIc(ic, icp, OpJz, 0, 0, 0, 0);

    ppc = blockCompile(tcBuf, icp, &ppc, var, ic); 

    if (tcBuf->tc[ppc] == TcElsif) {
        int32_t jmpIcp2 = *icp;
        putIc(ic, icp, OpJmp, 0, 0, 0, 0);
        putIc(ic, &jmpIcp1, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);
        ifControl(tcBuf, icp, &ppc, var, ic);
        putIc(ic, &jmpIcp2, OpJmp, (var_t *)((int64_t)*icp), 0, 0, 0);

    } else if (tcBuf->tc[ppc] == TcElse) {
        int32_t jmpIcp3 = *icp;
        putIc(ic, icp, OpJmp, 0, 0, 0, 0);
        putIc(ic, &jmpIcp1, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);

        ppc++; // else { の分2個進める

        ppc = blockCompile(tcBuf, icp, &ppc, var, ic);

        putIc(ic, &jmpIcp3, OpJmp, (var_t *)((int64_t)*icp), 0, 0, 0);

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
