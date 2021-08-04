#pragma once

#include <stdint.h>
#include "../utils/util.h"
#include "../variable/variable.h"

/* 認識するトークンの数の最大値 */
#define MAX_TC 4096

/* トークンコード列の大きさ */
#define TC_LIST_SIZE 20000

/* トークンの識別子 */ 
#define TcExpr         -1    // 式
#define TcIdentifier   -2    // 識別子(変数やラベル)
#define TcOperator     -3    // 演算子
#define TcStop         -99   // 都合により構文評価を止めたいとき

/* 字句解析が返す値(トークンコード)の一覧 */
enum TOKENCODE {
    TcLF = 0,    // \n
    TcComma,     // ,
    TcColon,     // :

    TcSqBrOpn,   // [
    TcSqBrCls,   // ]

    TcBrOpn,     // (
    TcBrCls,     // )

    TcLArrow,    // <-
    TcRArrow,    // ->

    TcEqu,       // =

    TcPlus,      // +
    TcMinus,     // -
    TcAster,     // *
    TcSlash,     // /
    TcPerce,     // %

    TcEEq,         // '=='
    TcNEq,         // '!='
    TcLt,          // '<'
    TcGe,          // '>='
    TcLe,          // '<='
    TcGt,          // '>'
    
    TcBegin,     // begin BEGIN
    TcEnd,       // end END
    
    TcIn,        // in IN
    TcOut,       // out OUT
    
    TcImport,    // import IMPORT
    TcDefine,    // define DEFINE
    TcChannel,   // channel CHANNEL
    TcSound,     // sound SOUND
    TcFilter,    // filter FILTER

    TcIf,        // if IF
    TcElsif,     // elsif ELSIF
    TcElse,      // else ELSE
    TcThen,      // then THEN
    TcLoop,      // loop LOOP

    TcAnd,       // and AND
    TcOr,        // or OR
    
    TcNot,       // not NOT

    TcPlay,      // play PLAY
    TcBpm,       // bpm BPM
    TcNote,      // note NOTE
    TcMute,      // mute MUTE
    
    TcPrint,     // print PRINT
    TcExit       // exit EXIT
};

/* トークンの番号、文字列、文字数を保持する構造体 */
struct token {
    int32_t tc;    // トークン番号
    int32_t tl;    // トークンの文字列の長さ
    str_t ts;    // トークンの内容(文字列)を記憶, tcsBuf(token.c内)へのポインタ
};

/* 変換したトークンコードの情報や、ソースコードをトークンコード列に変換したものを含む構造体 */
typedef struct tokenBuf {
    int32_t tc[TC_LIST_SIZE];    // ソースコードをトークンコード列に変換したものを入れる
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
 * 
 * args:
 *     s     : トークンの文字列
 *     len   : トークンの長さ
 *     tcBuf : トークンコードの情報
 *     var   : 変数リスト
 *     type  : (定数の場合)型
 */
int32_t getTc(str_t s, int32_t len, tokenBuf_t *tcBuf, var_t *var, int32_t type);

/* 予約語かどうか調べる */
int32_t isRsvWordTc(tokenBuf_t *tcBuf, int32_t tc);

/* 演算子記号などを最初にlexerしておく関数 */
int32_t tcInit(tokenBuf_t *tcBuf, var_t *var);