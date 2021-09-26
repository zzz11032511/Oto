#pragma once

#include <stdio.h>
#include <assert.h>

char *src_open(const char *filename);
size_t count_string_length(char *s, char endch);

#define GET_ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))
#define CALC_TIME(s, e) ((double)(e - s) / CLOCKS_PER_SEC)

#ifdef DEBUG

#define TEST_EQ(a, b) assert(a == b)
#define TEST_NE(a, b) assert(a != b)

#define DEBUG_IPRINT(v) do { \
    printf(#v); \
    printf(" : %d\n", v); \
} while (0)
#define DEBUG_FPRINT(v) do { \
    printf(#v); \
    printf(" : %f\n", v); \
} while (0)

#else

#define TEST_EQ(a, b) while (0) {}
#define TEST_NE(a, b) while (0) {}

#define DEBUG_IPRINT(v) while (0) {}
#define DEBUG_FPRINT(v) while (0) {}

#endif
