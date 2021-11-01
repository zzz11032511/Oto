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

#include "oto_const.h"
#include "oto_sound.h"

/* util.c */

// Token comparison macros

#define IS_RSVWORD(tc)        ((TC_BEGIN <= tc) && (tc <= TC_EXIT))
#define IS_AVAILABLE_VAR(tc)  (tc > TC_EXIT)
#define IS_SYMBOL(tc)         ((TC_LF <= tc) && (tc < TC_BEGIN))
#define IS_NOT_SYMBOL(tc)     (tc >= TC_BEGIN)
#define IS_INSTRUCTION(tc)    ((TC_PRINT <= tc) && (tc < TC_EXIT))
#define IS_ARITH_OPERATOR(tc) ((TC_PLUS <= tc && tc <= TC_GT) || (TC_AND <= tc && tc <= TC_OR))

char *src_open(const char *path);
size_t count_file_size(const char *path);
bool is_otofile(const char *path);

char to_lower(char ch);
char to_upper(char ch);

// strncmp case-insensitive
int32_t strncmp_cs(const char *str1, const char *str2, size_t maxcount);

// strcmp case-insensitive
int32_t strcmp_cs(const char *str1, const char *str2);

// Vector<uint64_t>
typedef struct {
    uint64_t *data;
    size_t length;
    size_t capacity;
} VectorUI64;

VectorUI64 *new_vector_ui64(size_t capacity);
void free_vector_ui64(VectorUI64 *vec);
void append_vector_ui64(VectorUI64 *vec, uint64_t data);
void set_vector_ui64(VectorUI64 *vec, uint64_t idx, uint64_t data);

// Vector<uint64_t>
typedef struct {
    void **data;
    size_t length;
    size_t capacity;
} VectorPTR;

VectorPTR *new_vector_ptr(size_t capacity);
void free_vector_ptr(VectorPTR *vec);
void append_vector_ptr(VectorPTR *vec, void *data);
void set_vector_ptr(VectorPTR *vec, uint64_t idx, void *data);

// token_listの最大容量の初期値
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

// initial reservation words 

extern const Token symbols[];
extern const Token rsvwords[];

/* run.c */

void oto_init();
void oto_run(const int8_t *path);
void oto_quit();

void set_timecount_flag(bool flag);

/* token.c */

void init_token_list();
void init_rsvword();
void free_token_list();
tokencode_t allocate_tc(char *str, size_t len, tokentype_t type);
bool is_rsvword(char *str, size_t len);

VectorPTR *make_var_list();
void free_var_list(VectorPTR *var_list);
void print_var(VectorPTR *var_list);

/* lexer.c */

void tokenize(char *s, VectorUI64 *src_tokens);
VectorUI64 * lexer(char *s);

/* compile.c */

// opcodesの最大容量の初期値
#define DEFAULT_MAX_OPCODES 4096

// トークン化したソースコードを内部コード列に変換する
VectorPTR *compiler(VectorUI64 *src_tokens, VectorPTR *var_list);
void compiler_sub(uint64_t *icp, uint64_t start, uint64_t end);

/* exec.c */

void exec(VectorPTR *ic_list);