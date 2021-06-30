#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "compile.h"
#include "iStack.h"
#include "ic.h"
#include "lexer.h"
#include "token.h"
#include "util.h"
#include "variable.h"

/* 演算子のトークンコードを対応する内部コードに変換する */
int32_t tc2op(int32_t tc) {
    switch (tc) {
        case TcPlus:
            return OpAdd;
        case TcMinus:
            return OpSub;
        case TcAster:
            return OpMul;
        case TcSlash:
            return OpDiv;
        case TcPerce:
            return OpMod;
        default:
            // 一致しない(演算子でない)ときはOpNopを返す
            return OpNop;
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
    int32_t p1 = 1;
    int32_t p2 = 1;

    // TcEndのときは優先度-99とする
    if (tc1 == TcEnd) {
        p1 = -99;
    }
    if (tc2 == TcEnd) {
        p2 = -99;
    }

    if (tc1 == TcAster || tc1 == TcSlash || tc1 == TcWSlash ||
        tc1 == TcPerce) {
        p1 = 2;
    }
    if (tc2 == TcAster || tc2 == TcSlash || tc2 == TcWSlash ||
        tc2 == TcPerce) {
        p2 = 2;
    }

    return p1 - p2;
}

#define RPN_TC_LIST_SIZE 1000

/**
 *  トークンコード列を逆ポーランド記法に並び替える関数
 *
 *  args:
 *      start:  式を表すトークン列の先頭
 *      end:    式を表すトークン列の末尾
 *      rpnTc:  逆ポーランド記法に並べ替えたトークン列の配列
 *      rpnTcP: 逆ポーランド記法に並べ替えたトークン列をどこまで書いたか
 */
int32_t rpn(tokenBuf_t *tcBuf, int32_t start, int32_t end, int32_t *rpnTc,
            int32_t rpnTcP) {
    struct iStack stack;
    stack.sp = 0;

    // NULL参照を防ぐためにTcEndをpushしておく(push,popをもっと安全にすべき)
    // push(&stack, TcEnd);

    for (int32_t pc = start; pc < end; pc++) {
        int32_t tc = tcBuf->tc[pc];  // 現在指しているトークンを取ってくる

        if (tc > TcEnd) {
            // ただの変数,定数ならそのまま書き込む
            rpnTc[rpnTcP++] = tc;
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
                        // 「)」でもネストが合わないとき
                        nest--;
                    }
                } else if (ttc == TcBrOpn) {
                    // 「(」がまた1つ増えた
                    nest++;
                } else if (end1 > end) {
                    // 括弧が見つからないならsyntax error
                    fprintf(stderr, "syntax error\n");
                    exit(1);
                }
            }

            rpnTcP = rpn(tcBuf, start1, end1, rpnTc, rpnTcP);

            pc += end1 - start1;  // ()の中の分だけ進める

        } else if (TcEEq <= tc && tc <= TcEqu) {
            // 演算子のとき
            if (priorityCmp(tc, iPeek(&stack)) > 0) {
                iPush(&stack, tc);
                // printf("push : %d\n", tc);

            } else {
                // スタックの一番上の演算子よりも優先度が低いときには
                // スタックの先頭の優先度が低くなるか, spが0になるまで書き込む
                while (stack.sp > 0 && (priorityCmp(tc, iPeek(&stack)) <= 0)) {
                    rpnTc[rpnTcP++] = iPop(&stack);
                    // printf("pop : %d\n", rpnTc[rpnTcP - 1]);
                }
                iPush(&stack, tc);
                // printf("push : %d\n", tc);
            }
        }
    }

    while (stack.sp > 0) {
        // 最後にスタックに残ったものをpopする
        // stack.sp > 0でないのは, 冒頭のpush(TcEnd)の分
        rpnTc[rpnTcP++] = iPop(&stack);
    }

    return rpnTcP;
}


int32_t expr(tokenBuf_t *tcBuf, int32_t *icp, int32_t *pc, var_t *var,
             var_t **ic) {
    int32_t ppc = *pc;  // 最初のpcを保存しておく

    struct iStack varStack;  // 変数を入れるスタック
    varStack.sp = 0;

    int32_t start = *pc;

    int32_t i = start;
    while (tcBuf->tc[i] != TcSemi) i++;  // 式の終わりを探す
    int32_t end = i;

    int32_t rpnTc[RPN_TC_LIST_SIZE];  // 逆ポーランド記法に書き替えたトークン列

    int32_t rpnTcN = rpn(tcBuf, start, end, rpnTc,
                         0);  // 逆ポーランド記法に書き替えたトークン列の長さ

    // デバッグ用
    // printf("rpnTc : ");
    // for (int32_t i = 0; i < rpnTcN; i++) {
    //     printf("%d ", rpnTc[i]);
    // }
    // printf("\n");

    int32_t t1 = 0;
    int32_t t2 = 0;
    for (int32_t i = 0; i < rpnTcN; i++) {
        int32_t tc = rpnTc[i];

        if (TcEEq <= tc && tc <= TcEqu) {
            // tcが演算子のときはputIc()する
            int32_t op = tc2op(tc);

            varStack.sp -= 2;

            putIc(ic, icp, op, 0, 0, 0, 0);

        } else if (tc > TcEnd) {
            // そうじゃなかったらスタックに積む
            putIc(ic, icp, OpPush, &var[tc], 0, 0, 0);
            iPush(&varStack, tc);
        }
    }

    *pc = ppc + (end - start) + 1;  // PCを(式の長さ + 1)分進める

    return 0;
}