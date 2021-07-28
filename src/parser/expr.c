#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compile.h"
#include "../debug.h"
#include "../errorHandle.h"
#include "../utils/iStack.h"
#include "../utils/util.h"
#include "../vm/ic.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

#define IS_OPERATION(tc) (TcPlus <= tc && tc <= TcPerce)

/* 演算子のトークンコードを対応する内部コードに変換する */
int32_t tc2op(int32_t tc) {
    switch (tc) {
        case TcPlus:   return OpAdd;
        case TcMinus:  return OpSub;
        case TcAster:  return OpMul;
        case TcSlash:  return OpDiv;
        case TcPerce:  return OpMod;
        // case TcEEq:    return OpEq;
        // case TcNEq:    return OpNEq;
        // case TcLt:     return OpLtCmp;
        // case TcGe:     return OpRiEqCmp;
        // case TcLe:     return OpLtEqCmp;
        // case TcGt:     return OpRiCmp;
        // case TcAndAnd: return OpAnd;
        // case TcBarBar: return OpOr;
        default:
            // 一致しない(演算子でない)ときはOpNopを返す
            return OpNop;
    }
}

/* 演算子のトークンコードを優先度に変換する */
int32_t tc2priority(int32_t tc) {
    switch (tc) {
        case TcPlus:
        case TcMinus:
            return 7;
        case TcAster:
        case TcSlash:
        case TcPerce:
            return 8;
        // case TcEEq:
        // case TcNEq:
        // case TcLt:
        // case TcGe:
        // case TcLe:
        // case TcGt:
        //     return 6;
        // case TcAndAnd:
        //     return 5;
        // case TcBarBar:
        //     return 4;
        // TcEndのときは優先度-99とする
        case TcExit:
            return -99;
        default:
            // 一致しない(演算子でない)ときは-99を返す
            return -99;
    }    
}

/**
 *  演算子の優先度を比較する関数
 *
 *  examples:
 *      tc1 > tc2 ~ return 1;
 *      tc1 = tc2 ~ return 0;
 *      tc1 < tc2 ~ return -1;
 */
int32_t priorityCmp(int32_t tc1, int32_t tc2) {
    int32_t p1 = tc2priority(tc1);
    int32_t p2 = tc2priority(tc2);

    return p1 - p2;
}

#define RPN_TC_LIST_SIZE 1000

/* トークンコード列を逆ポーランド記法に並び替える関数 */
int32_t rpn(tokenBuf_t *tcBuf, int32_t start, int32_t end, int32_t *rpnTc, int32_t rpnTcP) {
    struct iStack stack;
    stack.sp = 0;

    // 直前のトークンが演算子だったか
    int32_t beforeOpe = 0;

    for (int32_t pc = start; pc < end; pc++) {
        int32_t tc = tcBuf->tc[pc];  // 現在指しているトークンを取ってくる

        if (tc >= TcEnd) {
            // ただの変数,定数ならそのまま書き込む
            rpnTc[rpnTcP++] = tc;
            beforeOpe = 0;
            continue;

        } else if (tc == TcBrOpn) {
            // もし括弧があったときは、括弧の範囲についてrpn()を再帰的に呼ぶ
            int32_t start1 = pc + 1;
            int32_t end1   = pc + 1;

            // 括弧の範囲を調べる
            int32_t nest = 0;
            while (1) {
                int32_t ttc = tcBuf->tc[end1++];
                if (ttc == TcBrCls) {
                    if (nest == 0) {
                        break;
                    } else {
                        nest--;
                    }
                } else if (ttc == TcBrOpn) {
                    nest++;
                } else if (end1 > end) {
                    // 括弧が見つからない
                    callError(INVALID_SYNTAX_ERROR);
                }
            }

            rpnTcP = rpn(tcBuf, start1, end1, rpnTc, rpnTcP);

            pc += end1 - start1;  // ()の中の分だけ進める

        } else if (IS_OPERATION(tc)) {
            if (beforeOpe) {
                callError(INVALID_SYNTAX_ERROR);
            } else {
                beforeOpe = 1;
            }

            if (priorityCmp(tc, iPeek(&stack)) > 0) {
                iPush(&stack, tc);

            } else {
                // スタックの一番上の演算子よりも優先度が低いときには
                // スタックの先頭の優先度が低くなるか, spが0になるまで書き込む
                while (stack.sp > 0 && (priorityCmp(tc, iPeek(&stack)) <= 0)) {
                    rpnTc[rpnTcP++] = iPop(&stack);
                }
                iPush(&stack, tc);
            }
        }
    }

    while (stack.sp > 0) {
        rpnTc[rpnTcP++] = iPop(&stack);
    }

    return rpnTcP;
}


int32_t expr(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var, var_t **ic, int32_t end) {
    int32_t ppc = *pc;  // 最初のpcを保存しておく

    struct iStack varStack;  // 変数を入れるスタック
    varStack.sp = 0;

    int32_t start = *pc;

    if (end == 0) {
        int32_t i = start;
        while (tcBuf->tc[i] != TcLF) i++;
        end = i;        
    } else {
        // expr()はendの"手前"まで変換を行うので、呼び出し元がこの仕様を考慮しなくてもいいように1足す
        end++;
    }

    int32_t rpnTc[RPN_TC_LIST_SIZE];  // 逆ポーランド記法に書き替えたトークン列

    int32_t rpnTcN = rpn(tcBuf, start, end, rpnTc, 0);  // 逆ポーランド記法に書き替えたトークン列の長さ

#ifdef DEBUG
    printRpnTc(tcBuf, rpnTc, rpnTcN);
#endif

    for (int32_t i = 0; i < rpnTcN; i++) {
        int32_t tc = rpnTc[i];

        if (IS_OPERATION(tc)) {
            // tcが演算子のときはputIc()する
            int32_t op = tc2op(tc);

            varStack.sp -= 2;

            putIc(ic, icp, op, 0, 0, 0, 0);

        } else if (tc >= TcEnd) {
            putIc(ic, icp, OpPush, &var[tc], 0, 0, 0);
            iPush(&varStack, tc);
        }
    }

    *pc = ppc + (end - start) + 1;  // PCを(式の長さ + 1)分進める

    return 0;
}