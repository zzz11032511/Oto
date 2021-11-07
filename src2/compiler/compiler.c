#include <oto.h>

#include "compiler.h"

/**
 * トークン化したソースコード, 変数リスト, 内部コード列は
 * よく参照するので, 楽に呼び出すためにcompile.c内全体で
 * 使えるようにする
 * TODO: compiler全体で使えるようにする
 */

SliceI64 *srctcs = NULL;
VectorPTR *vars  = NULL;
VectorPTR *ops   = NULL;

static void init_compile(SliceI64 *src_tokens, VectorPTR *var_list,
                         VectorPTR *opcodes) {
    srctcs = src_tokens;
    vars = var_list;
    ops = opcodes;
}

void put_opcode(int64_t *icp, opcode_t op, Var *v1, Var *v2, Var *v3, Var *v4) {
    vector_ptr_set(ops, (*icp)++, (Var *)op);
    vector_ptr_set(ops, (*icp)++, v1);
    vector_ptr_set(ops, (*icp)++, v2);
    vector_ptr_set(ops, (*icp)++, v3);
    vector_ptr_set(ops, (*icp)++, v4);
}

/* ptn_cmp()で一致したトークンを一時的に格納する */
#define TMPVARS_LENGTH 6
static tokencode_t tmpvars[TMPVARS_LENGTH];
static int64_t vp = 1;   // tmpvarsへのポインタ(1が先頭)

/* トークンのパターン識別子 */ 
#define PTN_END         -1  // 終端
#define PTN_LABEL       -2   // 変数かラベル
#define PTN_EXPR        -3   // 式
#define PTN_INST        -4   // 命令

static void reset_tmpvars() {
    for (int32_t i = 0; i < TMPVARS_LENGTH; i++) {
        tmpvars[i] = 0;
    }
    vp = 1;
}

/* src[i]から続くトークン列が指定したパターンと一致していればtrue */
static bool ptn_cmp(int64_t i, const int64_t *pattarn) {
    reset_tmpvars();

    int64_t ptn = 0;
    while (pattarn[ptn] != PTN_END) {
        tokencode_t tc = SRC(i);
        
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

const int64_t PTNS_DEFINE[] = {TC_DEFINE, PTN_LABEL, TC_COLON, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_OSCILDEF[] = {PTN_LABEL, TC_EQU, TC_OSCIL, TC_SQBROPN, PTN_LABEL, TC_SQBRCLS, TC_LF, PTN_END};
const int64_t PTNS_SOUNDDEF[] = {PTN_LABEL, TC_EQU, TC_SOUND, TC_SQBROPN, PTN_LABEL, TC_SQBRCLS, TC_LF, PTN_END};
const int64_t PTNS_CPYD[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_ADD2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_PLUS, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_SUB2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_MINUS, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_MUL2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_ASTER, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_DIV2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_SLASH, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_MOD2[] = {PTN_LABEL, TC_EQU, PTN_LABEL, TC_PERCE, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_CPY_EXPR[] = {PTN_LABEL, TC_EQU, PTN_EXPR, PTN_END};
const int64_t PTNS_LOOP[] = {TC_LOOP, PTN_END};
const int64_t PTNS_LABEL_ONLY[] = {PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_PRINT[] = {TC_PRINT, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_EXIT[] = {TC_EXIT, TC_LF, PTN_END};

void assign_to_literal_error_check(tokencode_t tc) {
    tokentype_t type = VAR(tc)->token->type;
    if (type == TK_TY_LITERAL || type == TK_TY_RSVWORD || type == TK_TY_SYMBOL) {
        oto_error_exit(OTO_ASSIGN_TO_LITERAL_ERROR);
    }
}

void compile_sub(int64_t *icp, int64_t start, int64_t end) {
    int64_t i = start;

    while (i < end) {
        if (SRC(i) == TC_LF) {
            i++;

        } else if (ptn_cmp(i, PTNS_DEFINE)) {
            assign_to_literal_error_check(tmpvars[1]);
            if (VAR(tmpvars[2])->type != TY_CONST) {
                oto_error_exit(OTO_DEFINE_ERROR);
            }
            VAR(tmpvars[1])->value = VAR(tmpvars[2])->value;
            VAR(tmpvars[1])->type  = TY_CONST;
            i += 5;

        } 
        // else if (ptn_cmp(i, PTN_OSCILDEF)) {
        //     i++
            
        // } else if (ptn_cmp(i, PTN_SOUNDDEF)) {

        // } 
        else if (ptn_cmp(i, PTNS_CPYD)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_CPYD, VAR(tmpvars[1]), VAR(tmpvars[2]), 0, 0);
            i += 4;

        } else if (ptn_cmp(i, PTNS_ADD2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_ADD2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_SUB2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_SUB2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_MUL2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_MUL2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_DIV2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_DIV2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_MOD2)) {
            assign_to_literal_error_check(tmpvars[1]);
            put_opcode(icp, OP_MOD2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_CPY_EXPR)) {
            assign_to_literal_error_check(tmpvars[1]);
            SliceI64 *exprtcs = make_line_tokencodes(srctcs, i + 2);
            expr(icp, exprtcs, vars);

            put_opcode(icp, OP_CPYP, VAR(tmpvars[1]), 0, 0, 0);

            /* "<Var> =" の分だけ+2 */
            i += exprtcs->length + 2;
            free_slice_i64(exprtcs);

        } else if (ptn_cmp(i, PTNS_LOOP)) {
            compile_loop(icp, &i);

        }
        // } else if (ptn_cmp(i, TC_IF, PTN_END)) {
        //     i++;

        // } else if (ptn_cmp(i, TC_FUNC, PTN_END)) {
        //     i++;

        // } else if (ptn_cmp(i, TC_TRACK, PTN_END)) {
        //     i++;

        // } else if (ptn_cmp(i, TC_FILTER, PTN_END)) {
        //     i++;

        // } else if (ptn_cmp(i, PTN_INST, PTN_END)) {
        //     i++;

        // } 
        else if (ptn_cmp(i, PTNS_LABEL_ONLY)) {
            i += 2;

        } else if (ptn_cmp(i, PTNS_PRINT)) {
            put_opcode(icp, OP_PRINT, VAR(tmpvars[1]), 0, 0, 0);
            i += 2;

        } else if (ptn_cmp(i, PTNS_EXIT)) {
            put_opcode(icp, OP_EXIT, 0, 0, 0, 0);
            i += 2;

        } else {
            oto_error_exit(OTO_INVALID_SYNTAX_ERROR);
        }
    }
}

#define DEFAULT_MAX_OPCODES 4096

VectorPTR *compile(VectorI64 *src_tokens, VectorPTR *var_list) {
    VectorPTR *opcodes = new_vector_ptr(DEFAULT_MAX_OPCODES);
    if (IS_NULL(opcodes)) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    // 式や制御構文の解析でスライスの方が扱いやすい
    SliceI64 *srctcs_slice = new_slice_i64(src_tokens, 0, src_tokens->length);
    init_compile(srctcs_slice, var_list, opcodes);

    // opcodeをどこまで書き込んだか
    int64_t icp = 0;
    compile_sub(&icp, 0, src_tokens->length);

    free_slice_i64(srctcs_slice);

    return opcodes;
}
