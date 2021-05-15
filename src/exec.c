#include <stdio.h>
#include "lexer.h"
#include "compile.h"

#define TC_LIST_SIZE 10000

/* 最初にlexerしておく文字列 */
char tcInit[] = "; . !!* 0 1 2 3 4 5 6 7 8 9 ( ) [ ] { } == != < >= <= > + - * / // % ++ -- = -> , !!** !!*** _t0 _t1 _t2 _t3 _t4 _t5 _t6 _t7 _t8 _t9";

int run(String src) {
    int tc[TC_LIST_SIZE];    // トークンコード列
    int var[TC_MAX + 1];     // 変数

    lexer(tcInit, tc, var);    // あらかじめtcInitをlexer()する

    if (compile(src, tc, var) < 0)
        return 1;

    // exec();
}
