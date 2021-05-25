#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "util.h"
#include "token.h"
#include "lexer.h"
#include "variable.h"
#include "compile.h"


#define MAX_STACK_SIZE 1000

struct iStack {
    int q[MAX_STACK_SIZE];
    int sp;
};

void push(struct iStack *stack, int value)
{
    stack->q[stack->sp++] = value;
}

int pop(struct iStack *stack)
{
    stack->sp--;
    return stack->q[stack->sp];
}

int peek(struct iStack *stack)
{
    int i = stack->sp - 1;
    return stack->q[i];
}


/**
 *  演算子の優先度を比較する関数
 * 
 *  examples:
 *      tc1 > tc2 ~ return 1;
 *      tc1 = tc2 ~ return 0;
 *      tc1 < tc2 ~ return -1;
 */
int priorityCmp(int tc1, int tc2)
{
    int p1 = 1;
    int p2 = 1;

    // TcEndのときは優先度-99とする
    if (tc1 == TcEnd) {
        p1 = -99;
    }
    if (tc2 == TcEnd) {
        p2 = -99;
    }

    if (tc1 == TcAster || tc1 == TcSlash || tc1 == TcWSlash || tc1 == TcPerce) {
        p1 = 2;
    }
    if (tc2 == TcAster || tc2 == TcSlash || tc2 == TcWSlash || tc2 == TcPerce) {
        p2 = 2;
    }

    return p1 - p2;
}

/**
 *  トークンコード列を逆ポーランド記法に並び替える関数
 * 
 *  args: 
 *      start: 式を表すトークン列の先頭
 *      end:   式を表すトークン列の末尾
 *      rpnTc: 逆ポーランド記法に並べ替えたトークン列の配列
 */
void rpn(tokenBuf_t *tcBuf, int start, int end, int *rpnTc)
{
    struct iStack stack;
    stack.sp = 0;

    int rpnTcP = 0;    // 逆ポーランド記法に並べ替えたトークン列をどこまで書いたか

    // NULL参照を防ぐためにTcEndをpushしておく(push,popをもっと安全にすべき)
    push(&stack, TcEnd);

    for (int pc = start; pc < end; pc++) {
        int tc = tcBuf->tc[pc];    // 現在指しているトークンを取ってくる

        if (tc > TcEnd) {
            // ただの変数,定数ならそのまま書き込む
            rpnTc[rpnTcP++] = tc;
            continue;

        } else if (TcEEq <= tc && tc <= TcEqu) {
            // 演算子のとき
            if (priorityCmp(tc, peek(&stack)) > 0) {
                push(&stack, tc);
                // printf("push : %d\n", tc);

            } else {
                // スタックの一番上の演算子よりも優先度が低いときには
                // スタックの先頭の優先度が低くなるか, spが0になるまで書き込む
                while (stack.sp > 0 && (priorityCmp(tc, peek(&stack)) <= 0)) {
                    rpnTc[rpnTcP++] = pop(&stack);
                    // printf("pop : %d\n", rpnTc[rpnTcP - 1]);
                }
                push(&stack, tc);
                // printf("push : %d\n", tc);
            }
        }
    }

    while (stack.sp > 1) {
        // 最後にスタックに残ったものをpopする
        // stack.sp > 0でないのは, 冒頭のpush(TcEnd)の分
        rpnTc[rpnTcP++] = pop(&stack);
    }
}

int expr(tokenBuf_t *tcBuf, int *pc, int *var, int **ic)
{
    int ppc = *pc; // 最初のpcを保存しておく

    struct iStack opStack;     // 演算子を入れるスタック
    struct iStack varStack;    // 変数を入れるスタック
    opStack.sp = 0;
    varStack.sp = 0;

    int start = *pc;

    int i = start;
    while (tcBuf->tc[i] != TcSemi) i++;    // 式の終わりを探す

    int end = i;
    int rpnTc[end - start];    // 逆ポーランド記法に書き替えたトークン列

    rpn(tcBuf, start, end, rpnTc);

    // デバッグ用
    for (int i = 0; i < (end - start); i++) {
        printf("rpnTc : %d\n", rpnTc[i]);
    }

    exit(1);

    return 0;
}