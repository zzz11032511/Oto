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

/* util.c */

char *src_open(const char *path);
size_t count_file_size(const char *path);
bool is_otofile(const char *path);

char to_lower(char ch);
char to_upper(char ch);

/* 大文字小文字を区別しないstrncmp() */
int32_t strncmp_cs(const char *str1, const char *str2, size_t maxcount);

/* 大文字小文字を区別しないstrcmp() */
int32_t strcmp_cs(const char *str1, const char *str2);

/* 大文字小文字を区別しないis_equal()<string> */
bool is_str_equal_cs(const char *str1, size_t s1len, const char *str2, size_t s2len);

/* Vector<uint64_t> */
typedef struct {
    int64_t *data;
    size_t length;
    size_t capacity;
} VectorI64;

VectorI64 *new_vector_i64(size_t capacity);
void free_vector_i64(VectorI64 *vec);
void vector_i64_append(VectorI64 *vec, int64_t data);
void vector_i64_set(VectorI64 *vec, int64_t idx, int64_t data);

/* Vector<pointer> */
typedef struct {
    void **data;
    size_t length;
    size_t capacity;
} VectorPTR;

VectorPTR *new_vector_ptr(size_t capacity);
void free_vector_ptr(VectorPTR *vec);

/* ベクタの要素をfreeする(結構危険) */
void free_items_vector_ptr(VectorPTR *vec);
void vector_ptr_append(VectorPTR *vec, void *data);
void vector_ptr_set(VectorPTR *vec, int64_t idx, void *data);

/* Slice<int64_t> */
typedef struct {
    int64_t *data;
    size_t length;
} SliceI64;

SliceI64 *new_slice_i64(VectorI64 *vec, int64_t start, int64_t end);
SliceI64 *new_slice_i64_from_slice(SliceI64 *org_slice, int64_t start, int64_t end);
void free_slice_i64(SliceI64 *slice);
int64_t slice_i64_get(SliceI64 *slice, int64_t idx);

/* Map */
typedef struct {
    VectorPTR *keys;
    VectorPTR *vals;
} Map;

Map *new_map();
void free_map();
void map_puti(Map *map, char *key, int64_t val);
int64_t map_geti(Map *map, char *key);
bool map_exist_key(Map *map, char *key);

/* 指定したキーのデータに1加算する */
void map_inc_val(Map *map, char *key);

/* 指定したキーのデータに1減算する */
void map_dec_val(Map *map, char *key);
void map_printi(Map *map);

/* Stack */
typedef struct {
    VectorPTR *datas;
    int64_t sp;
} Stack;

Stack *new_stack();
void free_stack(Stack *stack);
void stack_push(Stack *stack, void *data);
void stack_pushi(Stack *stack, int64_t data);
void *stack_pop(Stack *stack);
int64_t stack_popi(Stack *stack);
void *stack_peek(Stack *stack);
int64_t stack_peeki(Stack *stack);

/* run.c */

void oto_init(char *path);
void oto_run(const char *path);
void oto_exit_process();
void oto_error_exit(errorcode_t err);

void set_timecount_flag(bool flag);

/* token.c */

void init_token_list();
void init_rsvword();
void free_token_list();
tokencode_t allocate_tc(char *str, size_t len, tokentype_t type);
bool is_rsvword(char *str, size_t len);

VectorPTR *make_var_list();
void free_var_list(VectorPTR *var_list);

/* lexer.c */

void tokenize(char *src, VectorI64 *src_tokens);
VectorI64 * lexer(char *src);

/* preprocess.c */

void init_include_file_manager(char *root_path);
char *new_string_literal(char *src, int64_t idx) ;
void preprocess(char *src, int64_t idx, VectorI64 *src_tokens);

/* compile.c */

/* トークン化したソースコードを内部コード列に変換する */
VectorPTR *compile(VectorI64 *src_tokens, VectorPTR *var_list);

/* exec.c */

void exec(VectorPTR *ic_list);

/* debug.c */

void print_src_tokens(VectorI64 *src_tokens);
void print_rpn_tc(VectorI64 *rpntcs);
void print_var(VectorPTR *var_list);
void print_ic_list(VectorPTR *ic_list);

/* error.c */

void print_error(errorcode_t err);
