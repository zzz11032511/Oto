#include "oto.h"

char *new_string_literal(char *src, uint64_t idx) {
    if (src[idx] != '"') {
        return NULL;
    }
    idx++;

    size_t len = 0;
    while (src[idx + len] != '"') {
        if (src[idx + len] == '\0') {
            return NULL;
        }
        len++;
    }

    char *str = MYMALLOC(len + 1, char);
    if (IS_NULL(str)) {
        return NULL;
    }
    
    strncpy(str, &src[idx], len);

    return str;
}

static void include_file(char *src, uint64_t idx, VectorUI64 *src_tokens) {
    uint64_t idx2 = idx + 7;

    while (src[idx2] == ' ') {
        if (src[idx2] == '\n' || src[idx2] == '\0') {
            // pathが指定されていない
            return;
        }
        idx2++;
    }

    if (src[idx2] == '"') {

    }

}

void preprocess(char *src, uint64_t *idx, VectorUI64 *src_tokens) {
    uint64_t idx2 = *idx + 1;

    if (strcmp_cs("include", &src[idx2]) == 0) {
        include_file(src, idx2, src_tokens);
    }

    // preprocess文の終端まで飛ばす
    while (src[idx2] != '\n' && src[idx2] != '\0') {
        idx2++;
    }

    *idx = idx2;
}