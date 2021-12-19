#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

char *src_open(const char *path);
size_t count_file_size(const char *path);
bool is_otofile(const char *path);

char to_lower(char ch);
char to_upper(char ch);

void print_upto_char(char *str, char endch);

/* 大文字小文字を区別しないstrncmp() */
int32_t strncmp_cs(const char *str1, const char *str2, size_t maxcount);

/* 大文字小文字を区別しないstrcmp() */
int32_t strcmp_cs(const char *str1, const char *str2);

/* 大文字小文字を区別しないis_equal()<string> */
bool is_str_equal_cs(const char *str1, size_t s1len, const char *str2, size_t s2len);

/* Vector<uint64_t> */
typedef struct {
    int64_t *data;
    size_t length;
    size_t capacity;
} VectorI64;

VectorI64 *new_vector_i64(size_t capacity);
void free_vector_i64(VectorI64 *vec);
void vector_i64_append(VectorI64 *vec, int64_t data);
void vector_i64_set(VectorI64 *vec, int64_t idx, int64_t data);

/* Vector<pointer> */
typedef struct {
    void **data;
    size_t length;
    size_t capacity;
} VectorPTR;

VectorPTR *new_vector_ptr(size_t capacity);
void free_vector_ptr(VectorPTR *vec);

/* ベクタの要素をfreeする(結構危険) */
void free_items_vector_ptr(VectorPTR *vec);
void vector_ptr_append(VectorPTR *vec, void *data);
void vector_ptr_set(VectorPTR *vec, int64_t idx, void *data);

/* Slice<int64_t> */
typedef struct {
    int64_t *data;
    size_t length;
    int64_t abs_idx;  // Sliceの先頭の「元のVector」での位置
} SliceI64;

SliceI64 *new_slice_i64(VectorI64 *vec, int64_t start, int64_t end);
SliceI64 *new_slice_i64_from_slice(SliceI64 *org_slice, int64_t start, int64_t end);
void free_slice_i64(SliceI64 *slice);
int64_t slice_i64_get(SliceI64 *slice, int64_t idx);

/* Map */
typedef struct {
    VectorPTR *keys;
    VectorPTR *vals;
} Map;

Map *new_map();
void free_map();
void map_put(Map *map, char *key, void *val);
void map_puti(Map *map, char *key, int64_t val);
void *map_get(Map *map, char *key);
int64_t map_geti(Map *map, char *key);
bool map_exist_key(Map *map, char *key);

/* 指定したキーのデータに1加算する */
void map_inc_val(Map *map, char *key);

/* 指定したキーのデータに1減算する */
void map_dec_val(Map *map, char *key);
void map_printi(Map *map);

/* Stack */
typedef struct {
    VectorPTR *datas;
    int64_t sp;
} Stack;

Stack *new_stack();
void free_stack(Stack *stack);
void stack_push(Stack *stack, void *data);
void stack_pushi(Stack *stack, int64_t data);
void *stack_pop(Stack *stack);
int64_t stack_popi(Stack *stack);
void *stack_peek(Stack *stack);
int64_t stack_peeki(Stack *stack);