#include <oto.h>

void test_is_rsvword() {
    TEST_EQ(is_rsvword("begin", 5), true);
    TEST_EQ(is_rsvword("exit", 4), true);
    TEST_EQ(is_rsvword("papapa", 6), false);
    TEST_EQ(is_rsvword("BEGIN", 5), true);
    TEST_EQ(is_rsvword("EXIT", 5), true);
    TEST_EQ(is_rsvword("Begin", 5), true);
    TEST_EQ(is_rsvword("Began", 5), false);
    TEST_EQ(is_rsvword("export", 6), false);
}

int main(void) {
    //gcc -o test_token src2/test/test_token.c src2/util.c src2/token.c -I./include
    test_is_rsvword();
}