#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "parser_func.h"
#include "../opcode.h"
#include "../util/util.h"
#include "../util/int_stack.h"
#include "../error/error.h"
#include "../debug/debug.h"
#include "../token/token.h"
#include "../variable/type.h"
#include "../ic/ic.h"

static opcode_t tc2op(uint32_t tc) {
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

/* 演算子のトークンコードを優先度に変換する(数値が大きいほど優先度が高い) */
static uint32_t tc2priority(uint32_t tc) {
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
            return 0;
    }
}

/* tc1's priority - tc2's priority */
static int32_t priority_cmp(uint32_t tc1, uint32_t tc2) {
    uint32_t p1 = tc2priority(tc1);
    uint32_t p2 = tc2priority(tc2);

    return p1 - p2;
}


/* RPN トークンコード列の長さを返す */
static int32_t rpn(uint32_t begin, uint32_t end, tokencode_t *rpn_tc, uint32_t rpn_tcp) {
    struct int_stack stack;
    stack.sp = 0;

    // 直前のトークンが演算子だったか
    bool is_before_op = false;

    for (uint32_t cur = begin; cur < end; cur++) {
        tokencode_t tc = get_tc(cur);

        if (IS_VALID_NAME(tc)) {
            rpn_tc[rpn_tcp++] = tc;
            is_before_op = false;
            continue;

        } else if (tc == TcBrOpn) {
            uint32_t begin1 = cur;
            uint32_t end1   = seek_block_end(begin1);

            rpn_tcp = rpn(begin1 + 1, end1, rpn_tc, rpn_tcp);

            // ()の中の分だけ進める
            cur += end1 - begin1;

        } else if (IS_ARITH_OPERATOR(tc)) {
            if (is_before_op) {
                call_error(INVALID_SYNTAX_ERROR);
            } else {
                is_before_op = true;
            }

            if (priority_cmp(tc, ipeek(&stack)) > 0) {
                ipush(&stack, tc);
                continue;
            }

            // スタックの先頭の優先度が低くなるか, spが0になるまで書き込む
            while (stack.sp > 0 && (priority_cmp(tc, ipeek(&stack)) <= 0)) {
                rpn_tc[rpn_tcp++] = ipop(&stack);
            }
            ipush(&stack, tc);
        }
    }

    while (stack.sp > 0) {
        rpn_tc[rpn_tcp++] = ipop(&stack);
    }

    return rpn_tcp;
}

static void expr_sub(uint32_t *icp, tokencode_t *rpn_tc, size_t rpn_tc_len) {
    struct int_stack stack;
    stack.sp = 0;

    for (uint32_t i = 0; i < rpn_tc_len; i++) {
        tokencode_t tc = rpn_tc[i];

        if (IS_ARITH_OPERATOR(tc)) {
            opcode_t op = tc2op(tc);
            stack.sp -= 2;
            put_ic(icp, op, 0, 0, 0, 0);
        
        } else if (IS_VALID_NAME(tc)) {
            put_ic(icp, OpPush, VAR_P(tc), 0, 0, 0);
            ipush(&stack, tc);

        } else {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }
}

#define RPN_TC_LIST_SIZE 1000

void expr(uint32_t *cur, uint32_t end, uint32_t *icp) {
    uint32_t begin = *cur;

    if (end == 0) {
        end = begin;
        while (get_tc(end) != TcLF) end++;
    }

    tokencode_t rpn_tc[RPN_TC_LIST_SIZE];
    size_t rpn_tc_len = rpn(begin, end, rpn_tc, 0);

#ifdef DEBUG
    print_rpn_tc(rpn_tc, rpn_tc_len);
#endif

    expr_sub(icp, rpn_tc, rpn_tc_len);

    *cur += (end - begin) + 1;
}