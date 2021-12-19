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

#include "oto_const.h"
#include "oto_util.h"

#define DEFAULT_MAX_TC  4096

typedef struct {
    tokencode_t tc;
    char *str; 
    size_t len;
    tokentype_t type;
} Token;

typedef struct {
    Token *token;
    union value_u {
        int64_t i;
        double  f;
        void   *p;
    } value;
    vartype_t type;
} Var;

/* 記号一覧 */
extern const Token symbols[];

/* 予約語一覧 */
extern const Token rsvwords[];

/* トークン比較用マクロ */

#define IS_RSVWORD(tc)        ((TC_BEGIN <= tc) && (tc <= TC_EXIT))
#define IS_AVAILABLE_VAR(tc)  (tc > TC_EXIT)
#define IS_SYMBOL(tc)         ((TC_LF <= tc) && (tc < TC_BEGIN))
#define IS_NOT_SYMBOL(tc)     (tc >= TC_BEGIN)
#define IS_INSTRUCTION(tc)    ((TC_PRINT <= tc) && (tc < TC_EXIT))
#define IS_ARITH_OPERATOR(tc) ((TC_PLUS <= tc && tc <= TC_LE) || (TC_AND <= tc && tc <= TC_OR))

/* status.c */
typedef struct {
    /* flags */
    bool timecount_flag;
    bool repl_flag;

    char *root_srcpath;
    Map *srcfile_table;
    language_t language;
    int64_t sampling_rate;
} Status;

Status *get_oto_status();
void init_status(Status *status, char *srcpath);

/* run.c */

void oto_init(char *srcpath);
void oto_run();
void oto_exit_process();
void oto_error_exit(errorcode_t err);

// Read-Eval-Print Loop
void repl();

void set_timecount_flag(bool flag);
void set_repl_flag(bool flag);
bool get_repl_flag();
void set_language(language_t l);
language_t get_language();


/* token.c */

void init_token_list();
void init_rsvword();
void free_token_list();
tokencode_t allocate_tc(char *str, size_t len, tokentype_t type, VectorPTR *var_list);
bool is_rsvword(char *str, size_t len);

void add_new_variable(VectorPTR *var_list, Token *new_token);


/* lexer.c */

void tokenize(char *src, VectorI64 *src_tokens, VectorPTR *var_list, Status *status);
void lexer(char *src, VectorI64 *src_tokens, VectorPTR *var_list, Status *status);

/* preprocess.c */

void init_include_file_manager(char *root_path);
char *new_string_literal(char *src, int64_t idx) ;
void preprocess(char *src, int64_t idx, VectorI64 *src_tokens, VectorPTR *var_list, Status *status);

/* compile.c */

/* トークン化したソースコードを内部コード列に変換する */
VectorPTR *compile(VectorI64 *src_tokens, VectorPTR *var_list, char *src_str);

/* exec.c */

void exec(VectorPTR *ic_list, VectorPTR *var_list);

/* debug.c */

void print_src_tokens(VectorI64 *src_tokens);
void print_rpn_tc(VectorI64 *rpntcs);
void print_var(VectorPTR *var_list);
void print_ic_list(VectorPTR *ic_list);

/* error.c */

void print_error(errorcode_t err);

