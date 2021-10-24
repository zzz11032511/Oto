#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "tokencode.h"
#include "../variable/variable.h"
#include "../variable/type.h"

#define MAX_TC 4096
#define TC_LIST_SIZE 20000

typedef struct token {
    tokencode_t tc;  // トークン番号
    size_t      tl;  // トークンの文字列の長さ
    int8_t    *ts;  // トークンの内容(文字列)
} Token;

void set_tc(tokencode_t tc, uint32_t pc);

/* ソースコードを変換したトークン列から, 指定した場所のトークン番号を取得 */
tokencode_t get_tc(uint32_t pc);

/* ソースコードを変換したトークン列の長さ */
size_t get_conv_source_size();

/* 今までに変換したトークンの数 */
size_t get_conv_token_size();

/* 指定したトークンの文字列の長さを返す(デバッグ表示用に使う) */
size_t get_token_strlen(tokencode_t tc);

/* トークンの文字列を出力する(改行無し) */
void print_token_str(tokencode_t tc);

/* 新規のトークンに番号を割り当てる. 既存のものであればその番号を返す */
tokencode_t allocate_tc(int8_t *s, size_t len, type_t type);

/* 予約語・記号トークンを予め定義する */
void init_token();