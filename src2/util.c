#include "oto.h"

VectorUI64 *new_vector_ui64(size_t capacity) {
    VectorUI64 *vec = MYMALLOC1(VectorUI64);
    if (vec == NULL) {
        return NULL;
    }

    vec->length = 0;
    vec->capacity = capacity;
    vec->data = MYMALLOC(capacity, uint64_t);
    if (vec->data == NULL) {
        free(vec);
        return NULL;
    }

    return vec;
}

void free_vector_ui64(VectorUI64 *vec) {
    free(vec->data);
    free(vec);
}

static void realloc_vector_ui64(VectorUI64 *vec, size_t realloc_size) {
    uint64_t *new_data = realloc(vec->data, (sizeof(uint64_t) * realloc_size));
    if (new_data == NULL) {
        return;
    }
    
    vec->data = new_data;
    vec->capacity = realloc_size;

    return;
}

void append_vector_ui64(VectorUI64 *vec, uint64_t data) {
    if (vec->length >= vec->capacity) {
        realloc_vector_ui64(vec, vec->capacity + 10);
        // TODO: エラー処理を書く
    }

    vec->data[(vec->length)++] = data;
}

void set_vector_ui64(VectorUI64 *vec, uint64_t idx, uint64_t data) {
    if (vec->length >= vec->capacity) {
        realloc_vector_ui64(vec, vec->capacity + 10);
        // TODO: エラー処理を書く
    }

    vec->data[idx] = data;
}