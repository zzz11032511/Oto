#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compile.h"
#include "expr.h"
#include "../errorHandle.h"
#include "../utils/iStack.h"
#include "../utils/util.h"
#include "../vm/ic.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

int32_t getArgs(tokenBuf_t *tcBuf, int32_t start, int32_t end, int32_t *argv, int32_t argLen) {
    int32_t ppc = start + 1;
    int32_t argCnt = 0;

    while (ppc < end) {
        int32_t tc = tcBuf->tc[ppc];
        
        int32_t cnt = 0;
        while((ppc + cnt) < end && tcBuf->tc[ppc + cnt] != TcComma) cnt++;

        if (cnt >= 2)
            argv[argCnt++] = TcExpr;
        else
            argv[argCnt++] = tc;

        ppc += cnt;
    }

    if (argLen < argCnt) {
        callError(SYNTAX_ERROR);
    }

    return argCnt;
}

/* "then"の位置を渡すと、それに対応した"elsif", "else", "end"の位置を返す */
int32_t searchIFBlockEnd(tokenBuf_t *tcBuf, int32_t pc) {
    int32_t nest = 1;

    while (1) {
        int32_t tc = tcBuf->tc[pc++];
        if (tc == TcIf || tc == TcBegin) {
            nest++;
            continue;
        } else if (tc == TcElsif && nest == 1) {
            break;
        } else if (tc == TcElse && nest == 1) {
            break;
        } else if (tc == TcEnd) {
            nest--;
            if (nest != 0) continue;
            else break;
        } else if (tc == TcExit) {
            callError(INVALID_SYNTAX_ERROR);
        }
    }

    return pc - 1;
}

int32_t searchElseBlockEnd(tokenBuf_t *tcBuf, int32_t pc) {
    int32_t nest = 1;

    while (1) {
        int32_t tc = tcBuf->tc[pc++];
        if (tc == TcIf || tc == TcBegin) {
            nest++;
            continue;
        } else if (tc == TcEnd) {
            nest--;
            if (nest != 0) continue;
            else break;
        } else if (tc == TcExit) {
            callError(INVALID_SYNTAX_ERROR);
        }
    }

    return pc - 1;
}

/* "begin", '['の位置を渡すと、それに対応した"end", ']'の位置を返す */
int32_t searchBlockEnd(tokenBuf_t *tcBuf, int32_t pc) {
    int32_t nest = 0;

    if (tcBuf->tc[pc] == TcThen) {
        return searchIFBlockEnd(tcBuf, pc);
    } else if (tcBuf->tc[pc] == TcElse) {
        return searchElseBlockEnd(tcBuf, pc);
    }

    while (1) {
        int32_t tc = tcBuf->tc[pc++];
        if (tc == TcBegin || tc == TcSqBrOpn) {
            nest++;
            continue;
        } else if (tc == TcEnd || tc == TcSqBrCls) {
            nest--;
            if (nest != 0) continue;
            else break;
        } else if (tc == TcExit) {
            callError(INVALID_SYNTAX_ERROR);
        }
    }

    return pc - 1;
}

/* "begin-end"で囲まれたブロック内のコードをコンパイルし、ブロックを抜けた場所のpcを返す */
int32_t blockCompile(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t start = *pc + 1;
    int32_t end   = searchBlockEnd(tcBuf, *pc) - 1;

#ifdef DEBUG
    printf("block : start [%d], end [%d]\n", start, end);
#endif

    compile_sub(tcBuf, var, ic, icp, start, end);
    return end + 2;    // ブロックの外に持っていくため2足す
}

void loopControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t ppc = *pc + 1;

    int32_t argv[] = {0, 0, 0};
    int32_t start = ppc;
    int32_t end = searchBlockEnd(tcBuf, start);

    int32_t argLen = getArgs(tcBuf, start, end, argv, GET_ARRAY_LENGTH(argv));
    ppc = end + 1;

    int32_t jmpIcp1 = *icp;
    putIc(ic, icp, OpLoop, 0, 0, 0, 0);

    ppc = blockCompile(tcBuf, icp, &ppc, var, ic);

    putIc(ic, icp, OpJmp, (var_t *)((int64_t)jmpIcp1), 0, 0, 0);

    putIc(ic, &jmpIcp1, OpLoop, (var_t *)((int64_t)*icp), &var[argv[0]], 0, 0);

    *pc = ppc;

    return; 
}

void ifControl(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic) {
    int32_t ppc = *pc + 1;

    int32_t notFlag = 0;
    if (tcBuf->tc[ppc] == TcNot) {
        notFlag = 1;
        ppc++;
    }

    // 条件式部分
    int32_t start = ppc;
    int32_t end = searchBlockEnd(tcBuf, start);
    start++;

    expr(tcBuf, icp, &start, var, ic, end - 1);
    ppc = end + 1;

    // 処理部分
    int32_t jmpIcp1 = *icp;
    putIc(ic, icp, OpJz, 0, 0, 0, 0);

    ppc = blockCompile(tcBuf, icp, &ppc, var, ic);

    if (tcBuf->tc[ppc - 1] == TcElsif) {
        ppc--;
        int32_t jmpIcp2 = *icp;
        putIc(ic, icp, OpJmp, 0, 0, 0, 0);

        if (notFlag) {
            putIc(ic, &jmpIcp1, OpJnz, (var_t *)((int64_t)*icp), 0, 0, 0);
        } else {
            putIc(ic, &jmpIcp1, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);
        }

        ifControl(tcBuf, icp, &ppc, var, ic);
        putIc(ic, &jmpIcp2, OpJmp, (var_t *)((int64_t)*icp), 0, 0, 0);

    } else if (tcBuf->tc[ppc - 1] == TcElse) {
        ppc--;
        int32_t jmpIcp3 = *icp;
        putIc(ic, icp, OpJmp, 0, 0, 0, 0);

        if (notFlag) {
            putIc(ic, &jmpIcp1, OpJnz, (var_t *)((int64_t)*icp), 0, 0, 0);
        } else {
            putIc(ic, &jmpIcp1, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);
        }

        ppc = blockCompile(tcBuf, icp, &ppc, var, ic);

        putIc(ic, &jmpIcp3, OpJmp, (var_t *)((int64_t)*icp), 0, 0, 0);

    } else {
        if (notFlag) {
            putIc(ic, &jmpIcp1, OpJnz, (var_t *)((int64_t)*icp), 0, 0, 0);
        } else {
            putIc(ic, &jmpIcp1, OpJz, (var_t *)((int64_t)*icp), 0, 0, 0);
        }

    }

    *pc = ppc;

    return; 
}