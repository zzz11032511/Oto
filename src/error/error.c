#include "error.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../debug/debug.h"
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

/* エラー表示用の変数 */
static str_t e_filename;
static str_t e_src;
static tokenbuf_t *e_tcbuf;
static var_t *e_var_list;
static var_t **e_ic;

void init_error() {
    e_filename = NULL;
    e_src      = NULL;
    e_tcbuf    = NULL;
    e_var_list = NULL;
    e_ic       = NULL;
}

void set_filename(str_t filename) {
    e_filename = filename;
}

void set_src(str_t src) {
    e_src = src;
}

void set_tcbuf(tokenbuf_t *tcBuf) {
    e_tcbuf = tcBuf;
}

void set_var_list(var_t *var_list) {
    e_var_list = var_list;
}

void set_ic(var_t **ic) {
    e_ic = ic;
}

void set_error_all(str_t filename, str_t src, tokenbuf_t *tcbuf, var_t *var_list, var_t **ic) {
    set_filename(filename);
    set_src(src);
    set_tcbuf(tcbuf);
    set_var_list(var_list);
    set_ic(ic);
}

void oto_quit(int32_t exit_status) {
    free(e_src);
    free_tokenbuf(e_tcbuf);

    e_src = NULL;
    e_tcbuf = NULL;
    
    exit(exit_status);
}

/* 実行時例外 */
void call_exception(int32_t exception_code) {
    fprintf(stderr, "Exception : ");
    switch (exception_code) {
    case ERROR:
        fprintf(stderr, "Error\n");
        break;
    case TYPE_ERROR:
        fprintf(stderr, "TypeError\n");
        break;
    case ZERO_DIVISION_ERROR:
        fprintf(stderr, "ZeroDivisionError\n");
        break;
    case STACK_OVERFLOW_ERROR:
        fprintf(stderr, "StackOverflowError\n");
        break;
    default:
        fprintf(stderr, "Exception : (Exceptioncode : %d)\n", exception_code);
        break; 
    }

    oto_quit(EXIT_FAILURE);
}

/* コンパイルエラー */
void call_error(int32_t error_code) {
    switch (error_code) {
    case INVALID_SYNTAX_ERROR:
        fprintf(stderr, "SyntaxError : invalid syntax\n");
        break;        
    case ASSIGN_TO_LITERAL_ERROR:
        fprintf(stderr, "SyntaxError : can't assign to literal\n");
        break;
    case NAME_ERROR:
        fprintf(stderr, "SyntaxError : can't use reserved words as variable names\n");
        break;
    default:
        fprintf(stderr, "SystemError : (Errorcode : %d)\n", error_code);
        break;        
    }

    oto_quit(EXIT_FAILURE);
}