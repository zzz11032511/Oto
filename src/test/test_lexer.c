#include <oto.h>

void test_lexer() {
    init_token_list();
    init_rsvword();

    char s[] = "a bb 12 = /* c a */ aaa a ; (] <= a";

    VectorI64 *src_tokens = new_vector_i64(DEFAULT_MAX_TC);
    tokenize(s, src_tokens);

    int64_t true_tc[] ={
        TC_EXIT + 1, TC_EXIT + 2, TC_EXIT + 3, TC_EQU, TC_EXIT + 4,
        TC_EXIT + 1, TC_LF, TC_BROPN, TC_SQBRCLS, TC_LE, TC_EXIT + 1
    };

    for (uint64_t i = 0; i < src_tokens->length; i++) {
        TEST_EQ_NOT_PRINT(src_tokens->data[i], true_tc[i]);
    }

    VectorPTR *var_list = make_var_list();
    // print_var(var_list);

    free_vector_i64(src_tokens);
    free_var_list(var_list);
    free_token_list();
}

int main(void) {
    test_lexer();
}
