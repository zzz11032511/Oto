#include "../oto.h"

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

void test_string() {
    TEST_EQ(to_lower('A'), 'a');
    TEST_EQ(to_lower('Z'), 'z');
    TEST_EQ(to_lower('m'), 'm');
    TEST_EQ(to_upper('a'), 'A');
    TEST_EQ(to_upper('z'), 'Z');
    TEST_EQ(to_upper('B'), 'B');

    TEST_EQ(strncmp_cs("ABC", "ABC", 3), 0);
    TEST_EQ(strncmp_cs("ABC", "abc", 3), 0);
    TEST_NE(strncmp_cs("abc", "aBi", 3), 0);
    TEST_EQ(strncmp_cs("abc", "aBi", 2), 0);

    TEST_EQ(strcmp_cs("ABC", "ABC"),  0);
    TEST_NE(strcmp_cs("ABC", "AB"),   0);
    TEST_NE(strcmp_cs("ABC", "ABCD"), 0);
    TEST_NE(strcmp_cs("BCD", "ABD"),  0);
}

void test_map() {
    Map *map = new_map();

    map_puti(map, "aaa.oto", 10);
    map_puti(map, "bbb.oto", 20);
    map_puti(map, "ccc.oto", 30);

    TEST_EQ(map_geti(map, "aaa.oto"), 10);
    TEST_EQ(map_geti(map, "bbb.oto"), 20);
    TEST_EQ(map_geti(map, "ccc.oto"), 30);

    map_inc_val(map, "bbb.oto");
    map_inc_val(map, "bbb.oto");
    TEST_EQ(map_geti(map, "bbb.oto"), 22);

    map_dec_val(map, "aaa.oto");
    map_dec_val(map, "ccc.oto");
    TEST_EQ(map_geti(map, "aaa.oto"), 9);
    TEST_EQ(map_geti(map, "ccc.oto"), 29);
    
    map_printi(map);
    printf("\n");

    free_map(map);
}

int main(void) {
    // gcc -o test_util src2/test/test_util.c src2/util.c -I./include 
    
    test_vector_ui64();
    test_fileio();
    test_map();
    test_string();


    printf("success\n");
}