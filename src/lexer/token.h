#pragma once

#include <stdint.h>
#include "../oto.h"
#include "../utils/util.h"
#include "../variable/variable.h"

/* トークンの番号、文字列、文字数を保持する構造体 */
struct token {
    uint32_t tc;    // トークン番号
    uint32_t tl;    // トークンの文字列の長さ
    str_t ts;    // トークンの内容(文字列)を記憶, tcsBuf(token.c内)へのポインタ
};

/* 変換したトークンコードの情報を含む構造体 */
typedef struct token_buffer {
    uint32_t tc_list[TC_LIST_SIZE];    // ソースコードをトークンコード列に変換したもの
    struct token *conv_tokens[MAX_TC];    // 変換済みトークンコード一覧
    uint32_t tcs;    // 今まで発行したトークンコードの個数
    uint32_t tcb;    // tcsBuf[]の未使用領域
} tokenbuf_t;

/* トークン保存領域を新しく作る */
tokenbuf_t *new_tokenbuf();

/* トークンのメモリ領域を解放する */
void free_tokenbuf(tokenbuf_t *tcbuf);

/* 予約語かどうか調べる */
bool_t is_rsvword_tc(tokenbuf_t *tcbuf, uint32_t tc);

/**
 * トークンコードを得るための関数
 * もし登録されていないなら, 新しく作る
 */
uint32_t get_tc(tokenbuf_t *tcbuf, var_t *var_list, str_t s, uint32_t len, uint32_t type);

/* 演算子と予約語を最初にlexerしておく関数 */
void init_token(tokenbuf_t *tcbuf, var_t *var_list);

/* 変数を定義する命令 */
void define_var(tokenbuf_t *tcbuf, var_t *var_list, str_t name, int32_t type, double value);
