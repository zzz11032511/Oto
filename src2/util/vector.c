#include <oto.h>

#define VECTOR_REALLOC_SIZE 100

VectorI64 *new_vector_i64(size_t capacity) {
    VectorI64 *vec = MYMALLOC1(VectorI64);
    if (IS_NULL(vec)) {
        return NULL;
    }

    vec->length = 0;
    vec->capacity = capacity;
    vec->data = MYMALLOC(capacity, int64_t);
    if (IS_NULL(vec->data)) {
        free(vec);
        return NULL;
    }

    return vec;
}

void free_vector_i64(VectorI64 *vec) {
    if (IS_NULL(vec)) {
        return;
    }
    free(vec->data);
    free(vec);
}

static void realloc_vector_i64(VectorI64 *vec, size_t realloc_size) {
    int64_t *new_data = realloc(vec->data, (sizeof(int64_t) * realloc_size));
    if (IS_NULL(new_data)) {
        return;
    }
    
    vec->data = new_data;
    vec->capacity = realloc_size;

    return;
}

void vector_i64_append(VectorI64 *vec, int64_t data) {
    if (vec->length >= vec->capacity) {
        realloc_vector_i64(vec, vec->capacity + VECTOR_REALLOC_SIZE);
        // TODO: エラー処理を書く
    }

    vec->data[(vec->length)++] = data;
}

void vector_i64_set(VectorI64 *vec, int64_t idx, int64_t data) {
    // TODO: エラー処理を書く
    if (vec->length >= vec->capacity) {
        if (idx >= vec->length) {
            realloc_vector_i64(vec, idx + VECTOR_REALLOC_SIZE);
        } else {
            realloc_vector_i64(vec, vec->capacity + VECTOR_REALLOC_SIZE);
        }
    }

    vec->data[idx] = data;
    if (idx >= vec->length) {
        vec->length = idx + 1;
    }
}

/* Vector<pointer> */

VectorPTR *new_vector_ptr(size_t capacity) {
    VectorPTR *vec = MYMALLOC1(VectorPTR);
    if (IS_NULL(vec)) {
        return NULL;
    }

    vec->length = 0;
    vec->capacity = capacity;
    vec->data = MYMALLOC(capacity, void *);
    if (IS_NULL(vec->data)) {
        free(vec);
        return NULL;
    }

    return vec;
}

void free_vector_ptr(VectorPTR *vec) {
    if (IS_NULL(vec)) {
        return;
    }
    free(vec->data);
    free(vec);
}

void free_items_vector_ptr(VectorPTR *vec) {
    for (int64_t i = 0; i < vec->length; i++) {
        free(vec->data[i]);
        vec->data[i] = NULL;
    }
    free(vec->data);
    free(vec);
}

static void realloc_vector_ptr(VectorPTR *vec, size_t realloc_size) {
    void *new_data = realloc(vec->data, (sizeof(void *) * realloc_size));
    if (IS_NULL(new_data)) {
        return;
    }
    
    vec->data = new_data;
    vec->capacity = realloc_size;

    return;
}

void vector_ptr_append(VectorPTR *vec, void *data) {
    if (vec->length >= vec->capacity) {
        realloc_vector_ptr(vec, vec->capacity + VECTOR_REALLOC_SIZE);
        // TODO: エラー処理を書く
    }

    vec->data[(vec->length)++] = data;
}

void vector_ptr_set(VectorPTR *vec, int64_t idx, void *data) {
    // TODO: エラー処理を書く
    if (vec->length >= vec->capacity) {
        if (idx >= vec->length) {
            realloc_vector_ptr(vec, idx + VECTOR_REALLOC_SIZE);
        } else {
            realloc_vector_ptr(vec, vec->capacity + VECTOR_REALLOC_SIZE);
        }
    }

    vec->data[idx] = data;
    if (idx >= vec->length) {
        vec->length = idx + 1;
    }
}