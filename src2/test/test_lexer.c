#include "../oto.h"

void test_lexer() {
    init_token_list();

    init_rsvword();

    char s[] = "a bb 12 = /* c a */ aaa a ; (] <= a";

    VectorUI64 *vec = new_vector_ui64(DEFAULT_MAX_TC);
    tokenize(s, vec);

    for (uint32_t i = 0; i < vec->length; i++) {
        printf("%d ", vec->data[i]);
    }
    printf("\n");

    VectorPTR *var_list = make_var_list();
    print_var(var_list);

    free(vec);
    free(var_list);
    printf("success\n");
}

int main(void) {
    // gcc -o test_lexer src2/test/test_lexer.c src2/util.c src2/lexer.c src2/token.c -I./include
    test_lexer();
}