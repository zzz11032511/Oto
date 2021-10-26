#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "parser_func.h"
#include "../opcode.h"
#include "../util/util.h"
#include "../error/error.h"
#include "../token/token.h"
#include "../variable/type.h"
#include "../ic/ic.h"

/* PtnLabelで一致したトークンを一時的に格納する */
#define TMPVARS_LENGTH 6
tokencode_t tmpvars[TMPVARS_LENGTH];
uint32_t vp = 1;   // tmpvarsへのポインタ(1が先頭)

static void reset_tmpvars() {
    for (int32_t i = 0; i < TMPVARS_LENGTH; i++) {
        tmpvars[i] = 0;
    }
    vp = 1;
}

/* トークン列が指定したパターンと一致していればtrue */
static bool ptn_cmp(uint32_t cur, int32_t pattern, ...) {
    va_list ap;
    va_start(ap, pattern);
    reset_tmpvars();

    uint32_t ptn_tc = pattern;
    while (ptn_tc != PtnStop) {
        uint32_t tc = get_tc(cur);

        if (tc == ptn_tc) {
            // パターン一致
        } else if (ptn_tc == PtnInst && IS_COMMAND(tc)) {
            // 命令
        } else if (ptn_tc == PtnLabel && IS_NOT_SYMBOL(tc)) {
            tmpvars[vp++] = tc;
        }else if (ptn_tc == PtnExpr) {
            break;
        } else {
            va_end(ap);
            return false;
        }
        ptn_tc = va_arg(ap, uint32_t);
        cur++;
    }

    if (IS_RSVWORD(tmpvars[1]) || IS_RSVWORD(tmpvars[2]) 
        || IS_RSVWORD(tmpvars[3]) || IS_RSVWORD(tmpvars[4])) {
        // labelとして予約語が使われていたらエラー
        call_error(NAME_ERROR);
    }

    va_end(ap);
    return true;
}

#define ASSIGN_TO_LITERAL_ERROR_CHECK(tc) do { \
    if (get_type(tc) == TyConst) { \
        call_error(ASSIGN_TO_LITERAL_ERROR); \
    } \
} while (0)

void parser_sub(uint32_t *icp, uint32_t start, uint32_t end) {
    uint32_t cur = start;

    while (cur < end) {
        if (get_tc(cur) == TcLF) {
            cur++;

        } else if (ptn_cmp(cur, TcDefine, PtnLabel, TcColon, PtnLabel, TcLF, PtnStop)) {
            if (get_type(tmpvars[2]) != TyConst) {
                call_error(DEFINE_ERROR);
            }
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            assign_float(tmpvars[1], TyConst, get_fvalue(tmpvars[2]));
            cur += 5;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, TcSound, TcSqBrOpn, PtnLabel, TcSqBrCls, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpDefS, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), 0, 0);
            cur += 7;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, TcOscil, TcSqBrOpn, PtnLabel, TcSqBrCls, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpDefOsc, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), 0, 0);
            cur += 7;

        } else if (ptn_cmp(cur, PtnLabel, TcRArrow, PtnStop)) {
            parser_connect_filters(&cur, icp);

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, PtnLabel, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpCpyD, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), 0, 0);
            cur += 4;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, PtnLabel, TcPlus, PtnLabel, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpAdd2, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), VAR_P(tmpvars[3]), 0);
            cur += 6;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, PtnLabel, TcMinus, PtnLabel, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpSub2, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), VAR_P(tmpvars[3]), 0);
            cur += 6;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, PtnLabel, TcAster, PtnLabel, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpMul2, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), VAR_P(tmpvars[3]), 0);
            cur += 6;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, PtnLabel, TcSlash, PtnLabel, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpDiv2, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), VAR_P(tmpvars[3]), 0);
            cur += 6;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, PtnLabel, TcPerce, PtnLabel, TcLF, PtnStop)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            put_ic(icp, OpMod2, VAR_P(tmpvars[1]), VAR_P(tmpvars[2]), VAR_P(tmpvars[3]), 0);
            cur += 6;

        } else if (ptn_cmp(cur, PtnLabel, TcEqu, PtnExpr)) {
            ASSIGN_TO_LITERAL_ERROR_CHECK(tmpvars[1]);
            cur += 2;
            expr(&cur, 0, icp);
            put_ic(icp, OpCpyP, VAR_P(tmpvars[1]), 0, 0, 0);

        } else if (ptn_cmp(cur, TcLoop, PtnStop)) {
            parser_loop(&cur, icp);

        } else if (ptn_cmp(cur, TcIf, PtnStop)) {
            parser_if(&cur, icp);

        } else if (ptn_cmp(cur, PtnInst, PtnStop)) {
            parser_instruction(&cur, icp);

        } else if (ptn_cmp(cur, TcExit, TcLF, PtnStop)) {
            put_ic(icp, OpExit, 0, 0, 0, 0);
            cur += 2;

        } else if (ptn_cmp(cur, PtnLabel, TcLF, PtnStop)) {
            cur += 2;

        } else {
            call_error(INVALID_SYNTAX_ERROR);
        }
    }
}

void parser() {
    // icをどこまで書き込んだか
    uint32_t icp = 0;
    uint32_t end = get_conv_source_size();

    parser_sub(&icp, 0, end);

    put_ic(&icp, OpExit, 0, 0, 0, 0);
}