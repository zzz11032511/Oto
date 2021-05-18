#ifndef INCLUDED_LEXER
#define INCLUDED_LEXER 1

#include "util.h"
#include "token.h"

/* 字句解析が返す値(トークンコード)の一覧 */
enum {
    TcSemi = 0,    // ';'
    TcDot,         // '.'

    TcWiCard,      // ワイルドカード '!!*' この後ろにワイルドカードの番号を表す数字が入る
    Tc0, Tc1, Tc2, Tc3, Tc4, Tc5, Tc6, Tc7, Tc8, Tc9,   // ワイルドカードの番号

    TcBrOpn,       // 丸括弧 開 '('
    TcBrCls,       // 丸括弧 閉 '}'
    TcSqBrOpn,     // 四角括弧 開 '['
    TcSqBrCls,     // 四角括弧 閉 ']'
    TcCrBrOpn,     // 波括弧 開 '{'
    TcCrBrCls,     // 波括弧 閉 '}'

    TcEEq,         // '=='
    TcNEq,         // '!='
    TcLt,          // '<'
    TcGe,          // '>='
    TcLe,          // '<='
    TcGt,          // '>'

    TcPlus,        // '+'
    TcMinus,       // '-'
    TcAster,       // '*'
    TcSlash,       // '/'
    TcWSlash,      // '//' 切り捨てて整数を返す割算
    TcPerce,       // '%'
    TcPlPlus,      // '++'
    TcMiMinus,     // '--'
    TcEqu,         // '='

    TcArrow,       // '->' 関数の返り値を示すときに使う...つもり(ex. -> int)

    TcComma,       // ','

    TcExpr,        // 式用ワイルドカード
    TcExpr0,       // 式用ワイルドカード2
    TcTmp0, TcTmp1, TcTmp2, TcTmp3, TcTmp4, TcTmp5, TcTmp6, TcTmp7, TcTmp8, TcTmp9,    // 一時変数
};

int lexer(String s, tokenBuf_t *tcBuf);

#endif