#include "../oto.h"

void test_tokenise() {
    init_token_list();

    char s[] = "a bb 12 = /* c a */ aaa a ; (] <= a";

    VectorUI64 *vec = new_vector_ui64(DEFAULT_MAX_TC);
    tokenize(s, vec);

    for (uint32_t i = 0; i < vec->length; i++) {
        printf("tc[%d] = %d\n", i, vec->data[i]);
    }

    free(vec);
    printf("success\n");
}

int main(void) {
    //gcc -o test_tokenize src2/test/test_tokenize.c src2/util.c src2/lexer.c src2/token.c -I./include
    test_tokenise();
}