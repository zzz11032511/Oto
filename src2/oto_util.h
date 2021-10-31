#pragma once

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <mymacro.h>

// Token comparison macros

#define IS_RSVWORD(tc)        ((TC_BEGIN <= tc) && (tc <= TC_EXIT))
#define IS_AVAILABLE_VAR(tc)  (tc > TC_EXIT)
#define IS_SYMBOL(tc)         ((TC_LF <= tc) && (tc < TC_BEGIN))
#define IS_NOT_SYMBOL(tc)     (!IS_SYMBOL(tc))
#define IS_INSTRUCTION(tc)    ((TC_PRINT <= tc) && (tc < TC_EXIT))
#define IS_ARITH_OPERATOR(tc) ((TC_PLUS <= tc && tc <= TC_GT) || (TC_AND <= tc && tc <= TC_OR))

// File io functions

char *src_open(const char *path);
size_t count_file_size(const char *path);
bool is_otofile(const char *path);

// String functions

char to_lower(char ch);
char to_upper(char ch);

/* strncmp case-insensitive */
int32_t strncmp_cs(const char *str1, const char *str2, size_t maxcount);
int32_t strcmp_cs(const char *str1, const char *str2);

// Vector<uint64_t>

typedef struct {
    uint64_t *data;
    size_t length;
    size_t capacity;
} VectorUI64;

VectorUI64 *new_vector_ui64(size_t capacity);
void free_vector_ui64(VectorUI64 *vec);
void append_vector_ui64(VectorUI64 *vec, uint64_t data);
void set_vector_ui64(VectorUI64 *vec, uint64_t idx, uint64_t data);

// Vector<uint64_t>

typedef struct {
    void **data;
    size_t length;
    size_t capacity;
} VectorPTR;

VectorPTR *new_vector_ptr(size_t capacity);
void free_vector_ptr(VectorPTR *vec);
void append_vector_ptr(VectorPTR *vec, void *data);
void set_vector_ptr(VectorPTR *vec, uint64_t idx, void *data);