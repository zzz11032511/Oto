#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../lexer/token.h"
#include "../variable/variable.h"

/* exception */
enum EXCEPTION_CODE {
    ERROR = 0,              // 謎のエラー        
    TYPE_ERROR,             // 型の不一致
    ZERO_DIVISION_ERROR,    // 0除算
    STACK_OVERFLOW_ERROR    // 内部スタックのオーバーフロー
};

/* error */
enum ERROR_CODE {
    SYSTEM_ERROR,                   // 内部エラー
    FAILURE_MAKE_TOKENBUFFER_ERROR, // 内部エラー(tcBuf作成失敗)
    FAILURE_MAKE_TOKEN_ERROR,       // 内部エラー(トークン作成失敗)
    TOO_MANY_TOKEN_ERROR,           // 内部エラー(トークン作りすぎエラー)

    SYNTAX_ERROR,                   // 謎の構文エラー
    INVALID_SYNTAX_ERROR,           // 単純な構文エラー
    ASSIGN_TO_LITERAL_ERROR,        // 定数に代入しようとしたエラー
    DEFINE_ERROR,                   // define文で定数以外を指定したエラー
    NAME_ERROR,                     // 予約語を変数名にしたエラー
};

void init_error();
void set_error_all(str_t filename, str_t src, tokenbuf_t *tcbuf, var_t *var_list, var_t **ic);

/* 実行時例外呼び出し */
void call_exception(int32_t exception_code);

/* 構文エラー呼び出し */
void call_error(int32_t error_code);

/* 終了時はこの関数を呼ぶ */
void oto_quit(int32_t exit_status);
