#include <stdint.h>

#include "util/util.h"
#include "token/token.h"
#include "token/tokencode.h"
#include "variable/type.h"
#include "variable/variable.h"

#include "parser/parser.c"
#include "ic/ic.c"
#include "error/error.c"
#include "util/util.c"
#include "lexer/lexer.c"
#include "lexer/preprocess.c"
#include "token/token.c"
#include "variable/variable.c"

void test_lexer() {
    TEST_EQ(count_string_length("123 23", ' '), 3);
    TEST_EQ(count_string_length("123 23", '2'), 1);
    TEST_EQ(count_string_length("123 23", '\0'), 6);

    TEST_EQ(count_constant_len("123"), 3);
    TEST_EQ(count_constant_len("12.3"), 4);
    TEST_EQ(count_constant_len("12.22.3"), 5);

    TEST_EQ(count_varname_len("asd"), 3);
    TEST_EQ(count_varname_len("asd#"), 4);
    TEST_EQ(count_varname_len("asd#_1fds"), 9);
    TEST_EQ(count_varname_len("{"), 0);

    TEST_EQ(count_operator_len("<-"),  2);
    TEST_EQ(count_operator_len("->"),  2);
    TEST_EQ(count_operator_len("="),   1);
    TEST_EQ(count_operator_len("<="),  2);
    TEST_EQ(count_operator_len(">"),   1);
    TEST_NE(count_operator_len(">=="), 3);

    TEST_EQ(lexer("2 = sound"), 3);
    TEST_EQ((int64_t)get_fvalue(TcExit + 1), 2);
    TEST_EQ(get_tc(2), TcSound);

    TEST_EQ(preprocess("@aaa\n"), 5);
    TEST_EQ(preprocess("@import\n"), 8);
}

void test_init_token() {
    TEST_EQ(allocate_tc("\n",    1, TyVoid),  TcLF);
    TEST_EQ(allocate_tc("<-",    2, TyVoid),  TcLArrow);
    TEST_EQ(allocate_tc("<=",    2, TyVoid),  TcLe);
    TEST_EQ(allocate_tc("begin", 5, TyVoid),  TcBegin);
    TEST_EQ(allocate_tc("exit",  4, TyVoid),  TcExit);
    TEST_EQ(allocate_tc("SOUND", 5, TyVoid),  TcSound);
    TEST_EQ(allocate_tc("sound", 5, TyVoid),  TcSound);
}

void test_parser() {
    lexer("a = 1\n");
    TEST_EQ(ptn_cmp(0, -1, TcEEq, -1, -99), false);
    TEST_EQ(ptn_cmp(0, -1, TcEqu, -1, -99), true);
}

int main(void) {
    init_token();

    test_parser();

    printf("Complete\n");
}