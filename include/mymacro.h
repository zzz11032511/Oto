#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define GET_ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

#define MYMALLOC(num, type) (type *)calloc(num, sizeof(type))
#define MYMALLOC1(type) (type *)calloc(1, sizeof(type))

#define ARRAY_BEGIN_ADDR(array) (array)
#define ARRAY_END_ADDR(array)   (array + GET_ARRAY_LENGTH(array))

#define IS_NOT_NULL(ptr) ( (ptr) != NULL )

#define CALC_TIME(s, e) ((double)(e - s) / CLOCKS_PER_SEC)

/* Ex : TIME_COUNT(sum(1, 2)); */
#define TIME_COUNT(handle) do { \
    time_t start = clock(); \
    handle; \
    time_t end   = clock(); \
    printf("time : %f[s]\n", CALC_TIME(start, end)); \
} while (0) \

#ifdef DEBUG

#define TEST_PRINT(a, b, op) do { \
    printf(#a); \
    printf(" : %d %f | ", a, a); \
    printf(#b); \
    printf(" : %d %f | ", b, b); \
    printf("%s %s %s | ", #a, #op, #b); \
    assert(a op b); \
    printf("ok.\n"); \
} while (0) \

#define TEST_EQ(a, b) TEST_PRINT(a, b, ==)
#define TEST_NE(a, b) TEST_PRINT(a, b, !=)
#define TEST_GT(a, b) TEST_PRINT(a, b, >)
#define TEST_GE(a, b) TEST_PRINT(a, b, >=)
#define TEST_LT(a, b) TEST_PRINT(a, b, <)
#define TEST_LE(a, b) TEST_PRINT(a, b, <=)

#define DEBUG_IPRINT(v) do { \
    printf(#v); \
    printf(" : %d\n", v); \
} while (0)
#define DEBUG_FPRINT(v) do { \
    printf(#v); \
    printf(" : %f\n", v); \
} while (0)

#else

#define TEST_PRINT(a, b, op) while (0) {}

#define TEST_EQ(a, b) while (0) {}
#define TEST_NE(a, b) while (0) {}
#define TEST_GT(a, b) while (0) {}
#define TEST_GE(a, b) while (0) {}
#define TEST_LT(a, b) while (0) {}
#define TEST_LE(a, b) while (0) {}

#define DEBUG_IPRINT(v) while (0) {}
#define DEBUG_FPRINT(v) while (0) {}

#endif