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

// run.c

void oto_run(const int8_t *filename);
void oto_quit();

void set_timecount_flag(bool flag);

// token.c

void init_token();
tokencode_t allocate_tc(char *str, size_t len, tokentype_t type);
bool is_rsvword(char *str, size_t len);
tokencode_t get_rsvword_tc(char *str, size_t len);

VectorPTR *make_var_list();

// lexer.c
void tokenize(char *s, VectorUI64 *src_tokens);
void lexer(char *s, VectorUI64 *src_tokens, VectorPTR *var_list);