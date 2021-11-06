#include <oto.h>

Map *new_map() {
    Map *map = MYMALLOC1(Map);
    if (IS_NULL(map)) {
        return NULL;
    }

    map->keys = new_vector_ptr(DEFAULT_MAX_MAP_CAPACITY);
    map->vals = new_vector_ptr(DEFAULT_MAX_MAP_CAPACITY);
    if (IS_NULL(map->keys) || IS_NULL(map->vals)) {
        free_map(map);
        return NULL;
    }

    return map;
}

void free_map(Map *map) {
    free_vector_ptr(map->keys);
    map->keys = NULL;
    
    free_vector_ptr(map->vals);
    map->vals = NULL;

    free(map);
}

void map_puti(Map *map, char *key, int64_t val) {
    vector_ptr_append(map->keys, key);
    vector_ptr_append(map->vals, (void *)val);
}

int64_t map_geti(Map *map, char *key) {
    for (int64_t i = 0; i < map->keys->length; i++) {
        if (strcmp(map->keys->data[i], key) == 0) {
            return (int64_t)map->vals->data[i];
        }
    }
    return 0;
}

static int64_t map_get_idx(Map *map, char *key) {
    for (int64_t i = 0; i < map->keys->length; i++) {
        if (strcmp(map->keys->data[i], key) == 0) {
            return i;
        }
    }
    return -1;
}

bool map_exist_key(Map *map, char *key) {
    int64_t idx = map_get_idx(map, key);
    if (idx == -1) {
        return false;
    }
    return true;
}

void map_inc_val(Map *map, char *key) {
    int64_t idx = map_get_idx(map, key);
    if (idx < 0) {
        return;
    }
    
    int64_t new_data = (int64_t)map->vals->data[idx] + 1;
    map->vals->data[idx] = (void *)new_data;
}

void map_dec_val(Map *map, char *key) {
    int64_t idx = map_get_idx(map, key);
    if (idx < 0) {
        return;
    }
    
    int64_t new_data = (int64_t)map->vals->data[idx] - 1;
    map->vals->data[idx] = (void *)new_data;
}

void map_printi(Map *map) {
    for (int64_t i = 0; i < map->keys->length; i++) {
        printf("%s : %I64d\n",
            (char *)map->keys->data[i],
            (int64_t)map->vals->data[i]
        );
    }
}