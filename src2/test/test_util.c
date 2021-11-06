#include <oto.h>

void test_vector_i64() {
    size_t cap = 3;
    VectorI64 *vec = new_vector_i64(cap);

    TEST_EQ(vec->length, 0);
    TEST_EQ(vec->capacity, cap);

    for (uint64_t i = 0; i < 5; i++) {
        vector_i64_append(vec, i);
    }

    TEST_EQ(vec->length, 5);
    TEST_EQ(vec->capacity, cap + 100);

    TEST_EQ(vec->data[2], 2);
    TEST_EQ(vec->data[4], 4);

    vector_i64_set(vec, 2, 10);
    TEST_EQ(vec->data[2], 10);

    free_vector_i64(vec);
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
    TEST_NE(strncmp_cs("AB", "ABC", 3),  0);
    TEST_NE(strncmp_cs("ABC", "AB", 3),  0);
    TEST_EQ(strncmp_cs("ABC", "abc", 3), 0);
    TEST_NE(strncmp_cs("abc", "aBi", 3), 0);
    TEST_EQ(strncmp_cs("abc", "aBi", 2), 0);

    TEST_EQ(strcmp_cs("ABC", "ABC"),  0);
    TEST_EQ(strcmp_cs("abc", "ABC"),  0);
    TEST_NE(strcmp_cs("ABC", "AB"),   0);
    TEST_NE(strcmp_cs("AB", "ABC"),   0);
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

void test_slice() {
    VectorI64 *veci64 = new_vector_i64(10);

    for (int64_t i = 0; i < 10; i++) {
        vector_i64_append(veci64, i);
        TEST_EQ_NOT_PRINT(veci64->data[i], i);
    }

    SliceI64 *slice = new_slice_i64(veci64, 3, 7);
    TEST_EQ(slice->length, 4);

    for (int64_t i = 0; i < slice->length; i++) {
        TEST_EQ(slice_i64_get(slice, i), veci64->data[i + 3]);
    }

    TEST_EQ(slice_i64_get(slice, -1), 0);
    TEST_EQ(slice_i64_get(slice, 10), 0);
}

void test_stack() {
    Stack *stack = new_stack();

    printf("sp : %d\n", stack->sp);
    stack_pushi(stack, 1);
    printf("sp : %d\n", stack->sp);
    stack_pushi(stack, 2);
    printf("sp : %d\n", stack->sp);
    stack_pushi(stack, 3);
    printf("sp : %d\n", stack->sp);
    stack_pushi(stack, 4);
    printf("sp : %d\n", stack->sp);
    stack_pushi(stack, 5);
    printf("sp : %d\n", stack->sp);
    stack_pushi(stack, 6);
    printf("sp : %d\n", stack->sp);

    int64_t val = 0;
    
    val = stack_popi(stack);
    TEST_EQ(val, 6);
    val = stack_popi(stack);
    TEST_EQ(val, 5);
    val = stack_popi(stack);
    TEST_EQ(val, 4);
    TEST_EQ(stack_peeki(stack), 3);
    val = stack_popi(stack);
    TEST_EQ(val, 3);

    stack_pushi(stack, 10);
    stack_pushi(stack, 15);

    DEBUG_IPRINT(stack_peeki(stack));

    val = stack_popi(stack);
    TEST_EQ(val, 15);
    val = stack_popi(stack);
    TEST_EQ(val, 10);
    val = stack_popi(stack);
    TEST_EQ(val, 2);
    val = stack_popi(stack);
    TEST_EQ(val, 1);
    val = stack_popi(stack);
    TEST_EQ(val, 0);
    val = stack_popi(stack);
    TEST_EQ(val, 0);
}

int main(void) {
    // gcc -o test_util -D DEBUG src2/test/test_util.c src2/util/util.c src2/util/vector.c src2/util/map.c src2/util/stack.c src2/util/slice.c -I./include 
    
    test_vector_i64();
    test_fileio();
    test_map();
    test_string();
    test_slice();
    test_stack();

    printf("success\n");
}