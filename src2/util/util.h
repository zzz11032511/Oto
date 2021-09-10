#pragma once

#include <assert.h>

char *src_open(const char *filename);
size_t count_string_length(char *s, char endch);

#define GET_ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))
#define CALC_TIME(s, e) ((double)(e - s) / CLOCKS_PER_SEC)

#define TEST_EQ(a, b) assert(a == b)
#define TEST_NE(a, b) assert(a != b)