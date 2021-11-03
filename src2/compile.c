#include "oto.h"

/**
 * トークン化したソースコード, 変数リスト, 内部コード列は
 * よく参照するので, 楽に呼び出すためにcompile.c内全体で
 * 使えるようにする
 */
static VectorI64 *srctcs = NULL;
static VectorPTR *vars = NULL;
static VectorPTR *ops = NULL;

static void init_compile(VectorI64 *src_tokens, VectorPTR *var_list,
                         VectorPTR *opcodes) {
    srctcs = src_tokens;
    vars = var_list;
    ops = opcodes;
}

/* トークンや変数を取得するための便利マクロ */
#define SRC(idx) srctcs->data[idx]
#define VAR(tc)  ((Var *)(vars->data[tc]))

/* 内部コードを書き込むための便利関数 */
static void put_opcode(int64_t *icp, opcode_t op, Var *v1, Var *v2, Var *v3, Var *v4) {
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
    // tmpvarsのリセット
    reset_tmpvars();

    int64_t ptn = 0;
    while (pattarn[ptn] != PTN_END) {
        tokencode_t tc = SRC(i);

        // DEBUG_IPRINT(tc);
        // DEBUG_IPRINT(pattarn[ptn]);
        
        if (tc == pattarn[ptn]) {
            // パターン一致

        } else if (pattarn[ptn] == PTN_LABEL && IS_NOT_SYMBOL(tc)) {
            tmpvars[vp++] = tc;

        }  else if (pattarn[ptn] == PTN_INST && IS_INSTRUCTION(tc)) {
            // 命令

        } else if (pattarn[ptn] == PTN_EXPR) {
            break;

        } else {
            // printf("\n");
            return false;
        }
        ptn++;
        i++;
    }
    // printf("matched\n\n");

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
const int64_t PTNS_LABEL_ONLY[] = {PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_PRINT[] = {TC_PRINT, PTN_LABEL, TC_LF, PTN_END};
const int64_t PTNS_EXIT[] = {TC_EXIT, TC_LF, PTN_END};

void compile_sub(int64_t *icp, int64_t start, int64_t end) {
    int64_t i = start;

    while (i < end) {
        if (SRC(i) == TC_LF) {
            i++;

        } else if (ptn_cmp(i, PTNS_DEFINE)) {
            if (VAR(tmpvars[2])->type != TY_CONST) {
                printf("define error\n");
                return;
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
            put_opcode(icp, OP_CPYD, VAR(tmpvars[1]), VAR(tmpvars[2]), 0, 0);
            i += 4;

        } else if (ptn_cmp(i, PTNS_ADD2)) {
            put_opcode(icp, OP_ADD2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_SUB2)) {
            put_opcode(icp, OP_SUB2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_MUL2)) {
            put_opcode(icp, OP_MUL2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_DIV2)) {
            put_opcode(icp, OP_DIV2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        } else if (ptn_cmp(i, PTNS_MOD2)) {
            put_opcode(icp, OP_MOD2, VAR(tmpvars[1]), VAR(tmpvars[2]), VAR(tmpvars[3]), 0);
            i += 6;

        }
        // else if (ptn_cmp(i, PTN_LABEL, TC_EQU, PTN_EXPR, PTN_END)) {
        //     i++;

        // } else if (ptn_cmp(i, TC_LOOP, PTN_END)) {
        //     i++;

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
            i++;

        } else if (ptn_cmp(i, PTNS_PRINT)) {
            put_opcode(icp, OP_PRINT, VAR(tmpvars[1]), 0, 0, 0);
            i += 2;

        } else if (ptn_cmp(i, PTNS_EXIT)) {
            put_opcode(icp, OP_EXIT, 0, 0, 0, 0);
            i += 2;

        } else {
            // error
            printf("compile error\n");
            return;
        }
    }
}

VectorPTR *compile(VectorI64 *src_tokens, VectorPTR *var_list) {
    VectorPTR *opcodes = new_vector_ptr(DEFAULT_MAX_OPCODES);
    if (IS_NULL(opcodes)) {
        return NULL;
    }
    init_compile(src_tokens, var_list, opcodes);

    // opcodeをどこまで書き込んだか
    int64_t icp = 0;

    compile_sub(&icp, 0, src_tokens->length);

    return opcodes;
}