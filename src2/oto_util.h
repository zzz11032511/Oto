#pragma once

#include <assert.h>
#include <stdint.h>
#include <mymacro.h>

// Token comparison macros

#define IS_RSVWORD(tc)        ((TcBegin <= tc) && (tc <= TcExit))
#define IS_AVAILABLE_VAR(tc)  (tc > TcExit)
#define IS_SYMBOL(tc)         ((TcLF <= tc) && (tc < TcBegin))
#define IS_NOT_SYMBOL(tc)     (!IS_SYMBOL(tc))
#define IS_INSTRUCTION(tc)    ((TcPrint <= tc) && (tc < TcExit))
#define IS_ARITH_OPERATOR(tc) ((TcPlus <= tc && tc <= TcGt) || (TcAnd <= tc && tc <= TcOr))

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