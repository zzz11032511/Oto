#include <oto.h>

void test_is_rsvword() {
    TEST_EQ_NOT_PRINT(is_rsvword("begin", 5), true);
    TEST_EQ_NOT_PRINT(is_rsvword("exit", 4), true);
    TEST_EQ_NOT_PRINT(is_rsvword("papapa", 6), false);
    TEST_EQ_NOT_PRINT(is_rsvword("BEGIN", 5), true);
    TEST_EQ_NOT_PRINT(is_rsvword("EXIT", 5), true);
    TEST_EQ_NOT_PRINT(is_rsvword("Begin", 5), true);
    TEST_EQ_NOT_PRINT(is_rsvword("Began", 5), false);
    TEST_EQ_NOT_PRINT(is_rsvword("export", 6), false);
}

int main(void) {
    test_is_rsvword();
}
