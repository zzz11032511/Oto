#include "errorHandle.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "debug.h"
#include "vm/alu.h"
#include "vm/ic.h"
#include "utils/util.h"
#include "lexer/token.h"
#include "variable/vStack.h"
#include "variable/variable.h"

/* エラー表示用の変数 */
static str_t eFilename;
static str_t eSrc;
static tokenBuf_t *eTcBuf;
static var_t *eVar;
static var_t **eIc;

void initErrorHandle() {
    eFilename = NULL;
    eSrc = NULL;
    eTcBuf = NULL;
    eVar = NULL;
    eIc = NULL;
}

void setFname(str_t filename) {
    eFilename = filename;
}

void setSrc(str_t src) {
    eSrc = src;
}

void setTcBuf(tokenBuf_t *tcBuf) {
    eTcBuf = tcBuf;
}

void setVar(var_t *var) {
    eVar = var;
}

void setIc(var_t **ic) {
    eIc = ic;
}

/* 終了処理 */
void otoQuit(int32_t exit_status) {
    free(eSrc);
    freeTokenBuf(eTcBuf);

    eSrc = NULL;
    eTcBuf = NULL;
    
    exit(exit_status);
}

/* 実行時例外 */
void callException(int32_t exceptionCode) {
    fprintf(stderr, "Exception : ");
    switch (exceptionCode) {
    case ERROR:
        fprintf(stderr, "Error\n");
        break;
    case TYPE_ERROR:
        fprintf(stderr, "TypeError\n");
        break;
    case FLOAT_MODULO_ERROR:
        fprintf(stderr, "FloatModuloError\n");
        break;
    case ZERO_DIVISION_ERROR:
        fprintf(stderr, "ZeroDivisionError\n");
        break;
    case STACK_OVERFLOW_ERROR:
        fprintf(stderr, "StackOverflowError\n");
        break;
    }

    otoQuit(EXIT_FAILURE);
}

/* コンパイルエラー */
void callError(int32_t errorCode) {
    switch (errorCode) {
    case INVALID_SYNTAX_ERROR:
        fprintf(stderr, "SyntaxError : invalid syntax\n");
        break;        
    case ASSIGN_TO_LITERAL_ERROR:
        fprintf(stderr, "SyntaxError : can't assign to literal\n");
        break;
    case SYNTAX_ERROR:
        fprintf(stderr, "SyntaxError\n");
        break;
    default:
        fprintf(stderr, "SystemError : (Errorcode : %d)\n", errorCode);
        break;        
    }

    otoQuit(EXIT_FAILURE);
}