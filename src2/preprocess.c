#include "oto.h"

/* idxは最初の「"」の位置 */
char *new_string_literal(char *src, uint64_t idx) {
    if (src[idx] != '"') {
        return NULL;
    }
    idx++;

    size_t len = 0;
    while (src[idx + len] != '"') {
        len++;
        if (src[idx + len] == '\0') {
            return NULL;
        }
    }

    char *str = MYMALLOC(len + 1, char);
    if (IS_NULL(str)) {
        return NULL;
    }
    
    strncpy(str, &src[idx], len);

    return str;
}

static void include_file(char *src, uint64_t idx, VectorUI64 *src_tokens) {
    idx += 7;

    while (src[idx] == ' ') {
        idx++;
        if (src[idx] == '\n' && src[idx] == '\0') {
            // pathが指定されていない
            printf("include error1\n");
            return;
        }
    }

    char *path = NULL;
    if (src[idx] == '"') {
        path = new_string_literal(src, idx);
        if (IS_NULL(path)) {
            printf("include error2\n");
            return;
        }

    } else {
        // pathが正しく指定されていない
        printf("include error3\n");
        return;
    }

    char *new_src = src_open(path);
    if (IS_NULL(new_src)) {
        printf("include error4\n");
        return;
    }

    tokenize(new_src, src_tokens);

    free(new_src);
    free(path);
}

void preprocess(char *src, uint64_t idx, VectorUI64 *src_tokens) {
    idx++;

    if (strcmp_cs("include", &src[idx]) == 0) {
        include_file(src, idx, src_tokens);
    }
}