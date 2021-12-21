#include "compiler.h"

#define DEFAULT_RPN_TC_LIST_SIZE 1000

static opcode_t tc2op(tokencode_t tc) {
    switch (tc) {
        case TC_PLUS:   return OP_ADD;
        case TC_MINUS:  return OP_SUB;
        case TC_ASTER:  return OP_MUL;
        case TC_SLASH:  return OP_DIV;
        case TC_PERCE:  return OP_MOD;
        case TC_EEQ:    return OP_EQ;
        case TC_NEQ:    return OP_NEQ;
        case TC_LT:     return OP_LTCMP;
        case TC_GE:     return OP_RIEQCMP;
        case TC_LE:     return OP_LTEQCMP;
        case TC_GT:     return OP_RICMP;
        case TC_AND:    return OP_AND;
        case TC_OR:     return OP_OR;
        default:        return OP_NOP;
    }
}

/* 演算子のトークンコードを優先度に変換する(数値が大きいほど優先度が高い) */
static int32_t tc2priority(tokencode_t tc) {
    switch (tc) {
        case TC_PLUS:
        case TC_MINUS:
            return 7;
        case TC_ASTER:
        case TC_SLASH:
        case TC_PERCE:
            return 8;
        case TC_EEQ:
        case TC_NEQ:
        case TC_LT:
        case TC_GE:
        case TC_LE:
        case TC_GT:
            return 6;
        case TC_AND:
            return 5;
        case TC_OR:
            return 4;
        default:
            return 0;
    }
}

/* tc1's priority - tc2's priority */
static int32_t priority_cmp(tokencode_t tc1, tokencode_t tc2) {
    int32_t p1 = tc2priority(tc1);
    int32_t p2 = tc2priority(tc2);

    return p1 - p2;
}

static void rpn(SliceI64 *exprtcs, VectorI64 *rpntcs) {
    Stack *stack = new_stack();

    /* 直前のトークンが演算子だったかどうか */
    bool is_before_op = false;

    for (int64_t i = 0; i < exprtcs->length; i++) {
        tokencode_t tc = slice_i64_get(exprtcs, i);

        if (IS_AVAILABLE_VAR(tc)) {
            vector_i64_append(rpntcs, tc);
            is_before_op = false;
        
        } else if (tc == TC_BROPN) {
            SliceI64 *exprtcs2 = make_args_enclosed_br(exprtcs, i);
            rpn(exprtcs2, rpntcs);
            i += exprtcs2->length;
            free_slice_i64(exprtcs2);
            is_before_op = false;

        } else if (IS_ARITH_OPERATOR(tc)) {
            if (is_before_op) {
                /* 演算子が連続していればエラー */
                error_compiler(OTO_INVALID_SYNTAX_ERROR, exprtcs, i);

            } else {
                is_before_op = true;
            }

            if (priority_cmp(tc, stack_peeki(stack)) > 0) {
                stack_pushi(stack, tc);
                continue;
            }

            /* スタックの先頭の優先度が低くなるか, spが0になるまで書き込む */
            while (stack->sp > 0 && priority_cmp(tc, stack_peeki(stack)) <= 0) {
                vector_i64_append(rpntcs, stack_popi(stack));
            }
            stack_pushi(stack, tc);
        }
    }

    /* 残っている演算子を全て書き込む */
    while (stack->sp > 0) {
        vector_i64_append(rpntcs, stack_popi(stack));
    }

    free_stack(stack);
}

static void expr_sub(int64_t *icp, VectorI64 *rpntcs, VectorPTR *vars) {
    for (int64_t i = 0; i < rpntcs->length; i++) {
        tokencode_t tc = rpntcs->data[i];

        if (IS_ARITH_OPERATOR(tc)) {
            opcode_t op = tc2op(tc);
            put_opcode(icp, op, 0, 0, 0, 0);

        } else if (IS_AVAILABLE_VAR(tc)) {
            put_opcode(icp, OP_PUSH, (Var *)(vars->data[tc]), 0, 0, 0);
        
        } else {
            oto_error(OTO_UNKNOWN_ERROR);
        }
    }
}

/* idxは式の先頭 */
void compile_expr(int64_t *icp, SliceI64 *exprtcs, VectorPTR *vars) {
    VectorI64 *rpntcs = new_vector_i64(DEFAULT_RPN_TC_LIST_SIZE);
    rpn(exprtcs, rpntcs);
#ifdef DEBUG
    print_rpn_tc(rpntcs);
#endif

    expr_sub(icp, rpntcs, vars);
    free_vector_i64(rpntcs);
}
