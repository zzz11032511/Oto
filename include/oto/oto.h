#pragma once

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <mymacro.h>
#include <time.h>
#include <windows.h>
#include <setjmp.h>

#include "oto_const.h"
#include "oto_util.h"

typedef struct {
    // flags
    bool timecount_flag;
    bool repl_flag;

    char *root_srcpath;
    Map *srcfile_table;
    language_t language;
    int64_t sampling_rate;
} Status;

typedef struct {
    tokencode_t tc;
    char *str; 
    size_t len;
    tokentype_t type;
} Token;

extern Token symbols[];   // 記号一覧
extern Token rsvwords[];  // 予約語一覧

/* トークン比較用マクロ */
#define IS_RSVWORD(tc)        ((TC_BEGIN <= tc) && (tc <= TC_EXIT))
#define IS_AVAILABLE_VAR(tc)  (tc > TC_EXIT)
#define IS_SYMBOL(tc)         ((TC_LF <= tc) && (tc < TC_BEGIN))
#define IS_NOT_SYMBOL(tc)     (tc >= TC_BEGIN)
#define IS_INSTRUCTION(tc)    ((TC_PRINT <= tc) && (tc < TC_EXIT))
#define IS_ARITH_OPERATOR(tc) ((TC_PLUS <= tc && tc <= TC_LE) || (TC_AND <= tc && tc <= TC_OR))

typedef struct {
    Token *token;
    union value_u {
        int64_t i;
        double  f;
        void   *p;
    } value;
    vartype_t type;
} Var;

void oto_init(char *srcpath);
void init_option(Status *status, char *srcpath);

void oto_exit();
void oto_error(errorcode_t err);
void oto_error_throw(errorcode_t err);
void oto_run();

void repl();

Status *get_oto_status();
void print_error(errorcode_t err, Status *status);

/* token, variable */
void init_var_list(VectorPTR *var_list);
void free_var_list(VectorPTR *var_list);
tokencode_t allocate_tc(char *str, size_t len, tokentype_t type, VectorPTR *var_list);
void add_new_variable(VectorPTR *var_list, Token *new_token);

/* lexer */
void tokenize(char *src, VectorI64 *src_tokens, VectorPTR *var_list, Status *status);
VectorI64 *lexer(char *src, VectorPTR *var_list, Status *status);
void preprocess(char *src, int64_t idx, VectorI64 *src_tokens, VectorPTR *var_list, Status *status);

char *new_string_literal(char *src, int64_t idx);
void preprocess(char *src, int64_t idx, VectorI64 *src_tokens, VectorPTR *var_list, Status *status);

/* compiler */
VectorPTR *compile(VectorI64 *src_tokens, VectorPTR *var_list, char *src_str, Status *status);

/* exec */
void exec(VectorPTR *ic_list, VectorPTR *var_list);

/* debug print */
void print_src_tokens(VectorI64 *src_tokens);
void print_rpn_tc(VectorI64 *rpntcs);
void print_var(VectorPTR *var_list);
void print_ic_list(VectorPTR *ic_list);