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

    /* ここから予約語 */
    TcVOID,        // void
    TcINT,         // 32bit int
    TcFLOAT,       // 32bit float
    TcSTRUCT,      // struct
    TcARRAY,       // array
    TcIF,          // if
    TcELSE,        // else
    TcWHILE,       // while
    TcFOR,         // for
    TcBREAK,       // break
    TcCONTINUE,    // continue
    TcFUNC,        // func
    TcRETURN,      // return
};


/* トークンコードの最大値 */
#define MAX_TC 255    

extern unsigned char *ts[MAX_TC + 1];             // トークンの内容(文字列)を記憶
extern int tl[MAX_TC + 1];                        // トークンの長さ
extern unsigned char tcBuf[(MAX_TC + 1) * 10];    // トークン1つあたり平均10バイトを想定
extern int tcs = 0;                               // 今まで発行したトークンコードの個数
extern int tcb = 0;                               // tcBuf[]の未使用領域の個数

extern int var[MAX_TC + 1];    // 変数


/* 最初にlexerしておく文字列 */
// char tcInit[] = "; . !!* 0 1 2 3 4 5 6 7 8 9 ( ) [ ] { } == != < >= <= > + - * / // % ++ -- = -> , !!** !!*** _t0 _t1 _t2 _t3 _t4 _t5 _t6 _t7 _t8 _t9 void int float struct array if else while for break continue func return";
char tcInit[] = "; . !!* 0 1 2 3 4 5 6 7 8 9 ( ) [ ] { } == != < >= <= > + - * / // % ++ -- = -> , !!** !!*** _t0 _t1 _t2 _t3 _t4 _t5 _t6 _t7 _t8 _t9";    // 予約語の処理を書くまではこっち

int lexer(unsigned char *s, int tc[]);    // プログラムをトークンコード列に変換する