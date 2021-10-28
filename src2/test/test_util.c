#include "../oto_util.h"

void test_vector_ui64() {
    size_t cap = 3;
    VectorUI64 *vec = new_vector_ui64(cap);

    TEST_EQ(vec->length, 0);
    TEST_EQ(vec->capacity, cap);

    for (uint64_t i = 0; i < 5; i++) {
        append_vector_ui64(vec, i);
    }

    TEST_EQ(vec->length, 5);
    TEST_EQ(vec->capacity, cap + 10);

    TEST_EQ(vec->data[2], 2);
    TEST_EQ(vec->data[4], 4);

    set_vector_ui64(vec, 2, 10);
    TEST_EQ(vec->data[2], 10);

    free_vector_ui64(vec);
}

void test_fileio() {
    TEST_EQ(is_otofile("aaa.oto"), true);
    TEST_EQ(is_otofile("./abc/aaa.oto"), true);
    TEST_EQ(is_otofile("aaa.ot"), false);
    TEST_EQ(is_otofile(".oto"), true);
}

int main(void) {
    // gcc -o test_util src2/test/test_util.c src2/util.c -I./include 
    test_vector_ui64();
    test_fileio();
}