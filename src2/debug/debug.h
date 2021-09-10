#pragma once

#include <stdio.h>

#include "../token/token.h"

/* ソースコードを変換したトークン列を出力する */
void print_converted_source();

/* 全トークンを出力する */
void print_all_token();

/* 内部コードを出力する */
void print_opcodes();

/* 数式を逆ポーランド記法にしたものを出力する */
void print_rpn_tc(tokencode_t *rpn_tc_list, size_t len);