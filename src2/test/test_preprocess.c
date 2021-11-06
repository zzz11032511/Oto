#include <oto.h>

void test_new_string_literal() {
    char src[] = "ABC \"12345\" CBA\0";
    char *str = new_string_literal(src, 4);

    TEST_EQ_NOT_PRINT(strcmp("12345", str), 0);
}

int main(void) {
    test_new_string_literal();
}
