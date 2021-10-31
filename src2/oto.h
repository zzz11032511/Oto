#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "oto_util.h"
#include "oto_const.h"

#define DEFAULT_MAX_TC 4096

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

// run.c

void oto_init();
void oto_run(const int8_t *path);
void oto_quit();

void set_timecount_flag(bool flag);

// token.c

void init_token_list();
void init_rsvword();
tokencode_t allocate_tc(char *str, size_t len, tokentype_t type);
bool is_rsvword(char *str, size_t len);
tokencode_t get_rsvword_tc(char *str, size_t len);

VectorPTR *make_var_list();
void print_var(VectorPTR *var_list);

// lexer.c

void tokenize(char *s, VectorUI64 *src_tokens);
void lexer(char *s, VectorUI64 *src_tokens, VectorPTR *var_list);
