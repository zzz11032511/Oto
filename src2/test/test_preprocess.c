#include <oto.h>

void test_new_string_literal() {
    char src[] = "ABC \"12345\" CBA\0";

    char *str = new_string_literal(src, 4);

    printf("%s\n", str);

    printf("success\n");
}

int main(void) {
    // gcc -o test_preprocess src2/test/test_preprocess.c src2/util.c src2/lexer.c src2/token.c src2/preprocess.c -I./include
    test_new_string_literal();
}