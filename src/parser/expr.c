#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "../lexer/tokencode.h"
#include "../debug/debug.h"
#include "../error/error.h"
#include "../utils/int_stack.h"
#include "../utils/util.h"
#include "../vm/opcode.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include "../variable/variable.h"


/* 演算子のトークンコードを対応する内部コードに変換する */
uint32_t tc2op(uint32_t tc) {
    switch (tc) {
        case TcPlus:   return OpAdd;
        case TcMinus:  return OpSub;
        case TcAster:  return OpMul;
        case TcSlash:  return OpDiv;
        case TcPerce:  return OpMod;
        case TcEEq:    return OpEq;
        case TcNEq:    return OpNEq;
        case TcLt:     return OpLtCmp;
        case TcGe:     return OpRiEqCmp;
        case TcLe:     return OpLtEqCmp;
        case TcGt:     return OpRiCmp;
        case TcAnd:    return OpAnd;
        case TcOr:     return OpOr;
        default:       return OpNop;
    }
}

/* 演算子のトークンコードを優先度に変換する */
int32_t tc2priority(uint32_t tc) {
    switch (tc) {
        case TcPlus:
        case TcMinus:
            return 7;
        case TcAster:
        case TcSlash:
        case TcPerce:
            return 8;
        case TcEEq:
        case TcNEq:
        case TcLt:
        case TcGe:
        case TcLe:
        case TcGt:
            return 6;
        case TcAnd:
            return 5;
        case TcOr:
            return 4;
        default:
            return -99;
    }    
}

/* tc1's priority - tc2's priority */
int32_t priority_cmp(uint32_t tc1, uint32_t tc2) {
    int32_t p1 = tc2priority(tc1);
    int32_t p2 = tc2priority(tc2);

    return p1 - p2;
}

#define RPN_TC_LIST_SIZE 1000

/**
 * トークンコード列を逆ポーランド記法に並び替える関数
 * トークンコード列の長さを返す
 */
int32_t rpn(tokenbuf_t *tcbuf, uint32_t start, uint32_t end, uint32_t *rpn_tc, uint32_t rpn_tcp) {
    struct int_stack stack;
    stack.sp = 0;

    // 直前のトークンが演算子だったか
    bool_t is_before_op = false;

    for (uint32_t pc = start; pc < end; pc++) {
        uint32_t tc = tcbuf->tc_list[pc];  // 現在指しているトークンを取ってくる

        if (IS_VALID_NAME(tc)) {
            // ただの変数,定数ならそのまま書き込む
            rpn_tc[rpn_tcp++] = tc;
            is_before_op = false;
            continue;

        } else if (tc == TcBrOpn) {
            // もし括弧があったときは、括弧の範囲についてrpn()を再帰的に呼ぶ
            uint32_t start1 = pc + 1;
            uint32_t end1   = pc + 1;

            // 括弧の範囲を調べる
            uint32_t nest = 0;
            while (1) {
                uint32_t ttc = tcbuf->tc_list[end1++];
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
                    call_error(INVALID_SYNTAX_ERROR);
                }
            }

            rpn_tcp = rpn(tcbuf, start1, end1, rpn_tc, rpn_tcp);

            pc += end1 - start1;  // ()の中の分だけ進める

        } else if (IS_OPERATION(tc)) {
            if (is_before_op) {
                call_error(INVALID_SYNTAX_ERROR);
            } else {
                is_before_op = true;
            }

            if (priority_cmp(tc, ipeek(&stack)) > 0) {
                ipush(&stack, tc);

            } else {
                // スタックの一番上の演算子よりも優先度が低いときには
                // スタックの先頭の優先度が低くなるか, spが0になるまで書き込む
                while (stack.sp > 0 && (priority_cmp(tc, ipeek(&stack)) <= 0)) {
                    rpn_tc[rpn_tcp++] = ipop(&stack);
                }
                ipush(&stack, tc);
            }
        }
    }

    while (stack.sp > 0) {
        rpn_tc[rpn_tcp++] = ipop(&stack);
    }

    return rpn_tcp;
}


void expr(tokenbuf_t *tcbuf, uint32_t *pc, uint32_t end, var_t *var_list, var_t **ic, uint32_t *icp) {
    uint32_t ppc = *pc;  // 最初のpcを保存しておく

    struct int_stack stack;  // 変数を入れるスタック
    stack.sp = 0;

    uint32_t start = *pc;

    if (end == 0) {
        uint32_t i = start;
        while (tcbuf->tc_list[i] != TcLF) i++;
        end = i;        
    } else {
        // expr()はendの"手前"まで変換を行うので、呼び出し元がこの仕様を考慮しなくてもいいように1足す
        end++;
    }

    uint32_t rpn_tc[RPN_TC_LIST_SIZE];  // 逆ポーランド記法に書き替えたトークン列

    uint32_t rpn_tc_len = rpn(tcbuf, start, end, rpn_tc, 0);  // 逆ポーランド記法に書き替えたトークン列の長さ

#ifdef DEBUG
    print_rpn_tc(tcbuf, rpn_tc, rpn_tc_len);
#endif

    for (uint32_t i = 0; i < rpn_tc_len; i++) {
        uint32_t tc = rpn_tc[i];

        if (IS_OPERATION(tc)) {
            // tcが演算子のときはputIc()する
            uint32_t op = tc2op(tc);

            stack.sp -= 2;

            put_ic(ic, icp, op, 0, 0, 0, 0);

        } else if (IS_VALID_NAME(tc)) {
            put_ic(ic, icp, OpPush, &var_list[tc], 0, 0, 0);
            ipush(&stack, tc);
        }
    }

    *pc = ppc + (end - start) + 1;  // PCを(式の長さ + 1)分進める
}