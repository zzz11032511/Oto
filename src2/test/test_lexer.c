#include <oto.h>

void test_lexer() {
    init_token_list();
    init_rsvword();

    char s[] = "a bb 12 = /* c a */ aaa a ; (] <= a";

    VectorI64 *src_tokens = new_vector_i64(DEFAULT_MAX_TC);
    tokenize(s, src_tokens);

    for (uint64_t i = 0; i < src_tokens->length; i++) {
        printf("%d ", src_tokens->data[i]);
    }
    printf("\n");

    VectorPTR *var_list = make_var_list();
    print_var(var_list);

    free_vector_i64(src_tokens);
    free_var_list(var_list);
    free_token_list();

    printf("success\n");
}

int main(void) {
    // gcc -o test_lexer src2/test/test_lexer.c src2/util.c src2/lexer.c src2/token.c src2/debug.c -I./include
    test_lexer();
}