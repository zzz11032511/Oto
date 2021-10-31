#include "oto.h"

VectorUI64 *new_vector_ui64(size_t capacity) {
    VectorUI64 *vec = MYMALLOC1(VectorUI64);
    if (IS_NULL(vec)) {
        return NULL;
    }

    vec->length = 0;
    vec->capacity = capacity;
    vec->data = MYMALLOC(capacity, uint64_t);
    if (IS_NULL(vec->data)) {
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
    if (IS_NULL(new_data)) {
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
    // TODO: エラー処理を書く
    if (vec->length >= vec->capacity) {
        if (idx >= vec->length) {
            realloc_vector_ui64(vec, idx + 10);
        } else {
            realloc_vector_ui64(vec, vec->capacity + 10);
        }
    }

    vec->data[idx] = data;
    if (idx >= vec->length) {
        vec->length = idx + 1;
    }
}

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

void free_item_vector_ptr(VectorPTR *vec) {
    uint64_t i = 0;
    while (i < vec->length) {
        free(vec->data[i]);
        vec->data[i] = NULL;
    }
}

void free_vector_ptr(VectorPTR *vec) {
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

void append_vector_ptr(VectorPTR *vec, void *data) {
    if (vec->length >= vec->capacity) {
        realloc_vector_ptr(vec, vec->capacity + 10);
        // TODO: エラー処理を書く
    }

    vec->data[(vec->length)++] = data;
}

void set_vector_ptr(VectorPTR *vec, uint64_t idx, void *data) {
    // TODO: エラー処理を書く
    if (vec->length >= vec->capacity) {
        if (idx >= vec->length) {
            realloc_vector_ptr(vec, idx + 10);
        } else {
            realloc_vector_ptr(vec, vec->capacity + 10);
        }
    }

    vec->data[idx] = data;
    if (idx >= vec->length) {
        vec->length = idx + 1;
    }
}

static FILE *open_file(const char *path) {
    FILE *fp = fopen(path, "r");
    if (IS_NULL(fp)) {
        perror(path);
        return NULL;
    }
    return fp;
}

size_t count_file_size(const char *path) {
    FILE *fp = open_file(path);
    if (IS_NULL(fp)) {
        return 0;
    }

    size_t size = 0;
    for (;;) {
        int32_t ch = fgetc(fp);
        if (ch == EOF) {
            break;
        }
        size++;
    }

    fclose(fp);

    return size;
}

char *src_open(const char *path) {
    FILE *fp = open_file(path);
    if (fp == 0) {
        return NULL;
    }

    size_t fsize = count_file_size(path);
    if (fsize == 0) {
        return NULL;
    }

    // srcの終端に'\0'を入れるために(fsize + 1)
    char *src = MYMALLOC((fsize + 1), char);
    if (IS_NULL(src)) {
        return NULL;
    }

    fread(src, sizeof(char), fsize, fp);
    src[fsize] = 0;

    fclose(fp);

#ifdef DEBUG
    printf("Source file info\n");
    printf("name : %s, size : %I64d bytes\n\n", filename, fsize);
#endif

    return src;
}

bool is_otofile(const char *path) {
    const char *ext = strrchr(path, '.');

    if (strcmp(ext, ".oto") == 0) return true;
    return false;
}

char to_lower(char ch) {
    if ('A' <= ch && ch <= 'Z') {
        return ch + 0x20;
    }
    return ch;
}

char to_upper(char ch) {
    if ('a' <= ch && ch <= 'z') {
        return ch - 0x20;
    }
    return ch;
}

int32_t strncmp_cs(const char *str1, const char *str2, size_t maxcount) {
    uint64_t i = 0;
    while (i < maxcount) {
        if (to_lower(str1[i]) != to_lower(str2[i])) {
            return str1[i] - str2[i];
        }
        i++;
    }

    return 0;
}

int32_t strcmp_cs(const char *str1, const char *str2) {
    uint64_t i = 0;
    while (to_lower(str1[i]) == to_lower(str2[i])) {
        if (str1[i] == '\0') {
            return 0;
        }
        i++;
    }

    return str1[i] - str2[i];
}