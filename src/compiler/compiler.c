#include <oto.h>

#include "compiler.h"

char *src = NULL;
VectorPTR *vars = NULL;
VectorPTR *ops  = NULL;

void put_opcode(int64_t *icp, opcode_t op, Var *v1, Var *v2, Var *v3, Var *v4) {
    vector_ptr_set(ops, (*icp)++, (Var *)op);
    vector_ptr_set(ops, (*icp)++, v1);
    vector_ptr_set(ops, (*icp)++, v2);
    vector_ptr_set(ops, (*icp)++, v3);
    vector_ptr_set(ops, (*icp)++, v4);
}

#define TMPVARS_LENGTH 6
/* ptn_cmp()で一致したトークンを一時的に格納する */
static tokencode_t tmpvars[TMPVARS_LENGTH];
static int64_t vp = 1;   // tmpvarsへのポインタ(1が先頭)

static void reset_tmpvars() {
    for (int32_t i = 0; i < TMPVARS_LENGTH; i++) {
        tmpvars[i] = 0;
    }
    vp = 1;
}

/* トークンのパターン識別子 */ 
#define PTN_END         -1  // 終端
#define PTN_LABEL       -2   // 変数かラベル
#define PTN_EXPR        -3   // 式
#define PTN_INST        -4   // 命令

const int64_t PTNS_DEFINE[] = {TC_DEFINE, PTN_LABEL, TC_COLON, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_OSCILDEF[] = {PTN_LABEL, TC_EQU, TC_OSCIL, TC_SQBROPN, PTN_LABEL, TC_SQBRCLS, TC_LF, PTN_END};
const int64_t PTNS_SOUNDDEF[] = {PTN_LABEL, TC_EQU, TC_SOUND, TC_SQBROPN, PTN_LABEL, TC_SQBRCLS, TC_LF, PTN_END};
const int64_t PTNS_CPYD[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_ADDCPY[] = {PTN_LABEL, TC_PLUSEQ, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_SUBCPY[] = {PTN_LABEL, TC_MINUEQ, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_MULCPY[] = {PTN_LABEL, TC_ASTEEQ, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_DIVCPY[] = {PTN_LABEL, TC_SLASEQ, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_MODCPY[] = {PTN_LABEL, TC_PERCEQ, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_ADD2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_PLUS, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_SUB2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_MINUS, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_MUL2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_ASTER, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_DIV2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_SLASH, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_MOD2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_PERCE, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_CPY_EXPR[] = {PTN_LABEL, TC_EQU, PTN_EXPR, PTN_END};
const int64_t PTNS_ADDCPY_EXPR[] = {PTN_LABEL, TC_PLUSEQ, PTN_EXPR, TC_LF, PTN_END};
const int64_t PTNS_SUBCPY_EXPR[] = {PTN_LABEL, TC_MINUEQ, PTN_EXPR, TC_LF, PTN_END};
const int64_t PTNS_MULCPY_EXPR[] = {PTN_LABEL, TC_ASTEEQ, PTN_EXPR, TC_LF, PTN_END};
const int64_t PTNS_DIVCPY_EXPR[] = {PTN_LABEL, TC_SLASEQ, PTN_EXPR, TC_LF, PTN_END};
const int64_t PTNS_MODCPY_EXPR[] = {PTN_LABEL, TC_PERCEQ, PTN_EXPR, TC_LF, PTN_END};
const int64_t PTNS_LOOP[] = {TC_LOOP, PTN_END};
const int64_t PTNS_IF[] = {TC_IF, PTN_END};
const int64_t PTNS_LABEL_ONLY[] = {PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_PRINT[] = {TC_PRINT, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_EXIT[] = {TC_EXIT, TC_LF, PTN_END};
const int64_t PTNS_INST[] = {PTN_INST, PTN_END};

/* src[i]から続くトークン列が指定したパターンと一致していればtrue */
static bool ptn_cmp(SliceI64 *srctcs, int64_t i, const int64_t *pattarn) {
    reset_tmpvars();

    int64_t ptn = 0;
    while (pattarn[ptn] != PTN_END) {
        tokencode_t tc = slice_i64_get(srctcs, i);
        
        if (tc == pattarn[ptn]) {
            // パターン一致

        } else if (pattarn[ptn] == PTN_LABEL && IS_NOT_SYMBOL(tc)) {
            tmpvars[vp++] = tc;

        }  else if (pattarn[ptn] == PTN_INST && IS_INSTRUCTION(tc)) {
            // 命令

        } else if (pattarn[ptn] == PTN_EXPR) {
            break;

        } else {
            return false;
        }
        ptn++;
        i++;
    }

    return true;
}

void assign_to_literal_error_check(tokencode_t tc) {
    tokentype_t type = VAR(tc)->token->type;
    if (type == TK_TY_LITERAL || type == TK_TY_RSVWORD || type == TK_TY_SYMBOL) {
        oto_error_exit(OTO_ASSIGN_TO_LITERAL_ERROR);
    }
}

void compile_sub(int64_t *icp, SliceI64 *srctcs, int64_t start, int64_t end) {
    int64_t i = start;

    while (i < end) {
        DEBUG_IPRINT(i);
        DEBUG_IPRINT(slice_i64_get(srctcs, i));
        if (slice_i64_get(srctcs, i) == TC_LF) {
            i++;

        } else if (ptn_cmp(srctcs, i, PTNS_DEFINE)) {
            assign_to_literal_error_check(tmpvars[1]);
            if (VAR(tmpvars[2])->type != TY_CONST) {
                oto_error_exit(OTO_DEFINE_ERROR);
            }
            VAR(tmpvars[1])->value = VAR(tmpvars[2])->value;
            VAR(tmpvars[1])->type  = TY_CONST;
            i += 5;

        } 
        // else if (ptn_cmp(srctcs, i, PTN_OSCILDEF)) {
        //     i++
        // } else if (ptn_cmp(srctcs, i, PTN_SOUNDDEF)) {
        // } 
        else if (ptn_cmp(srctcs, i, PTNS_CPYD)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_CPYD, VAR(tmpvars[1]), VAR(tmpvars[2]), 0, 0);
            i += 4;

        } else if (ptn_cmp(srctcs, i, PTNS_ADDCPY)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_ADD2, VAR(tmpvars[1]), VAR(tmpvars[1]), VAR(tmpvars[2]), 0);
            i += 4;

        } else if (ptn_cmp(srctcs, i, PTNS_SUBCPY)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_SUB2, VAR(tmpvars[1]), VAR(tmpvars[1]), VAR(tmpvars[2]), 0);
            i += 4;

        } else if (ptn_cmp(srctcs, i, PTNS_MULCPY)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_MUL2, VAR(tmpvars[1]), VAR(tmpvars[1]), VAR(tmpvars[2]), 0);
            i += 4;

        } else if (ptn_cmp(srctcs, i, PTNS_DIVCPY)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_DIV2, VAR(tmpvars[1]), VAR(tmpvars[1]), VAR(tmpvars[2]), 0);
            i += 4;

        } else if (ptn_cmp(srctcs, i, PTNS_MODCPY)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_MOD2, VAR(tmpvars[1]), VAR(tmpvars[1]), VAR(tmpvars[2]), 0);
            i += 4;

        } else if (ptn_cmp(srctcs, i, PTNS_ADD2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_ADD2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(srctcs, i, PTNS_SUB2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_SUB2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(srctcs, i, PTNS_MUL2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_MUL2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(srctcs, i, PTNS_DIV2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_DIV2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(srctcs, i, PTNS_MOD2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_MOD2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(srctcs, i, PTNS_CPY_EXPR)) {
            assign_to_literal_error_check(tmpvars[1]);
            SliceI64 *exprtcs = make_line_tokencodes(srctcs, i + 2);
            compile_expr(icp, exprtcs, vars);

            put_opcode(icp, OP_CPYP, VAR(tmpvars[1]), 0, 0, 0);

            // "<Var> =" の分だけ+2
            i += exprtcs->length + 2;
            free_slice_i64(exprtcs);

        } else if (ptn_cmp(srctcs, i, PTNS_ADDCPY_EXPR) || ptn_cmp(srctcs, i, PTNS_SUBCPY_EXPR)
                   || ptn_cmp(srctcs, i, PTNS_MULCPY_EXPR) || ptn_cmp(srctcs, i, PTNS_DIVCPY_EXPR)
                   || ptn_cmp(srctcs, i, PTNS_MODCPY_EXPR)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_PUSH, VAR(tmpvars[1]), 0, 0, 0);
            SliceI64 *exprtcs = make_line_tokencodes(srctcs, i + 2);
            compile_expr(icp, exprtcs, vars);

            tokencode_t op = slice_i64_get(srctcs, i + 1);
            if (op == TC_PLUSEQ) {
                put_opcode(icp, OP_ADD, 0, 0, 0, 0);
            } else if (op == TC_MINUEQ) {
                put_opcode(icp, OP_SUB, 0, 0, 0, 0);
            } else if (op == TC_ASTEEQ) {
                put_opcode(icp, OP_MUL, 0, 0, 0, 0);
            } else if (op == TC_SLASEQ) {
                put_opcode(icp, OP_DIV, 0, 0, 0, 0);
            } else if (op == TC_PERCEQ) {
                put_opcode(icp, OP_MOD, 0, 0, 0, 0);
            }
            put_opcode(icp, OP_CPYP, VAR(tmpvars[1]), 0, 0, 0);

            // "<Var> =" の分だけ+2
            i += exprtcs->length + 2;
            free_slice_i64(exprtcs);


        } else if (ptn_cmp(srctcs, i, PTNS_LOOP)) {
            compile_loop(icp, srctcs, &i);

        } else if (ptn_cmp(srctcs, i, PTNS_IF)) {
            compile_if(icp, srctcs, &i);

        } else if (ptn_cmp(srctcs, i, PTNS_INST)) {
            compile_instruction(icp, srctcs, &i);

        } else if (ptn_cmp(srctcs, i, PTNS_LABEL_ONLY)) {
            i += 2;

        } else if (ptn_cmp(srctcs, i, PTNS_PRINT)) {
            put_opcode(icp, OP_PRINT, VAR(tmpvars[1]), 0, 0, 0);
            i += 2;

        } else if (ptn_cmp(srctcs, i, PTNS_EXIT)) {
            put_opcode(icp, OP_EXIT, 0, 0, 0, 0);
            i += 2;

        } else {
            oto_error_exit(OTO_INVALID_SYNTAX_ERROR);
        }
    }
}

static void init_compile(VectorPTR *var_list, VectorPTR *opcodes, char *src_str) {
    vars = var_list;
    ops = opcodes;
    src = src_str;
}

#define DEFAULT_MAX_OPCODES 4096
VectorPTR *compile(VectorI64 *src_tokens, VectorPTR *var_list, char *src_str) {
    VectorPTR *opcodes = new_vector_ptr(DEFAULT_MAX_OPCODES);
    if (IS_NULL(opcodes)) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    // 式や制御構文の解析でスライスの方が扱いやすい
    SliceI64 *srctcs_slice = new_slice_i64(src_tokens, 0, src_tokens->length);
    init_compile(var_list, opcodes, src_str);

    // opcodeをどこまで書き込んだか
    int64_t icp = 0;
    compile_sub(&icp, srctcs_slice, 0, src_tokens->length);

    free_slice_i64(srctcs_slice);

    return opcodes;
}
