#pragma once

#include <stdint.h>
#include "variable.h"
#include "util.h"

// 認識するトークンの数の最大値
#define MAX_TC 1024

// トークンコード列の大きさ
#define TC_LIST_SIZE 10000

/* トークンの識別子 */ 
#define TcExpr         -1    // 式
#define TcType         -2    // 型
#define TcIdentifier   -3    // 識別子(変数やラベル)
#define TcConst        -4    // 定数
#define TcOperator     -5    // 演算子
#define TcStop         -99   // 都合により構文評価を止めたいとき

/* 字句解析が返す値(トークンコード)の一覧 */
enum {
    TcSemi = 0,    // ';'
    TcDot,         // '.'

    // 括弧
    TcBrOpn,       // 丸括弧 開 '('
    TcBrCls,       // 丸括弧 閉 ')'
    TcSqBrOpn,     // 四角括弧 開 '['
    TcSqBrCls,     // 四角括弧 閉 ']'
    TcCrBrOpn,     // 波括弧 開 '{'
    TcCrBrCls,     // 波括弧 閉 '}'

    // 比較演算子
    TcEEq,         // '=='
    TcNEq,         // '!='
    TcLt,          // '<'
    TcGe,          // '>='
    TcLe,          // '<='
    TcGt,          // '>'

    // 算術演算子
    TcPlus,        // '+'
    TcMinus,       // '-'
    TcAster,       // '*'
    TcSlash,       // '/'
    TcWSlash,      // '//' 切り捨てて整数を返す割算
    TcPerce,       // '%'
    TcEqu,         // '='

    // 論理演算子(あんま使わない)
    TcAndAnd,      // '&&'
    TcBarBar,      // '||'
    
    TcPlPlus,      // '++'
    TcMiMinus,     // '--'

    TcArrow,       // '->' 関数の返り値を示すときに使う...つもり(ex. -> int32_t)

    TcComma,       // ','

    // 予約語
    TcInt,         // int32_t型
    TcFloat,       // float型
    
    TcIf,          // if文
    TcPrint,       // print文

    TcEnd          // トークン定義がここまでだよーということを示す
};


struct token {
    int32_t tc;    // トークン番号
    int32_t tl;    // トークンの文字列の長さ
    str_t ts;    // トークンの内容(文字列)を記憶, tcsBuf(token.c内)へのポインタ
};


typedef struct tokenBuf {
    int32_t tc[TC_LIST_SIZE];    // 文字列をトークンコード列に変換したものを入れる
    struct token *tokens[MAX_TC];    // 変換済みトークンコード一覧
    int32_t tcs;    // 今まで発行したトークンコードの個数
    int32_t tcb;    // tcsBuf[]の未使用領域
} tokenBuf_t;

/* トークン保存領域を新しく作る */
tokenBuf_t *newTokenBuf();

/* トークンのメモリ領域を解放する(トークンコード列もついでに解放してくれる) */
void freeTokenBuf(tokenBuf_t *tcBuf);

/**
 * トークンコードを得るための関数
 * もし登録されていないなら, 新しく作る
 */
int32_t getTc(str_t s, int32_t len, tokenBuf_t *tcBuf, var_t *var, int32_t type);

/* トークンの名前の開始アドレスを返す */
uint8_t *getTcName(tokenBuf_t *tcBuf, int32_t tc);

/* 演算子記号などを最初にlexerしておく関数 */
int32_t tcInit(tokenBuf_t *tcBuf, var_t *var);