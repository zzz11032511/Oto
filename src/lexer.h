#ifndef INCLUDED_LEXER
#define INCLUDED_LEXER 1

#include "util.h"
#include "token.h"
#include "variable.h"

// トークンの識別子 
#define TcExpr         -1
#define TcType         -2
#define TcIdentifier   -3
#define TcConst        -4
#define TcOperator     -5

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

    TcPlPlus,      // '++'
    TcMiMinus,     // '--'

    TcArrow,       // '->' 関数の返り値を示すときに使う...つもり(ex. -> int)

    TcComma,       // ','

    // 予約語
    TcInt,         // int型
    TcFloat,       // float型
    
    TcPrint,
    
    // 一時変数
    TcTmp0, 
    TcTmp1, 
    TcTmp2, 
    TcTmp3, 
    TcTmp4, 
    TcTmp5, 
    TcTmp6, 
    TcTmp7, 
    TcTmp8, 
    TcTmp9,

    TcEnd          // トークン定義がここまでだよーということを示す
};

int lexer(String s, tokenBuf_t *tcBuf, int *var);

#endif