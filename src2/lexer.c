#include "oto.h"

static bool is_number(char c) {
    if ('0' <= c && c <= '9') return true;
    return false;
}

static bool is_varname(char c) {
    if ('0' <= c && c <= '9') return true;
    if ('a' <= c && c <= 'z') return true;
    if ('A' <= c && c <= 'Z') return true;
    if (c == '_') return true;
    if (c == '#') return true;
    return false;
}

static bool is_symbol_char(char c) {
    if (strchr("=+-*/%<>!", c) != 0) return true;
    return false;
}

static size_t count_constant_len(char *s) {
    size_t len = 0;
    bool is_float = false;

    while (1) {
        if (is_number(s[len])) {
            len++;
        } else if (s[len] == '.' && !is_float) {
            is_float = true;
            len++;
        } else {
            break;
        }
    }

    return len;
}

static size_t count_varname_len(char *s) {
    size_t len = 0;
    while (is_varname(s[len])) {
        len++;
    }

    return len;
}

static bool is_valid_operator(char *s, size_t len) {
    int64_t i = 0;
    while (symbols[i].str != NULL) {
        if (strcmp(s, symbols[i].str) == 0) {
            return true;
        }
        i++;
    }

    return false;
}

static size_t count_operator_len(char *s) {
    size_t len = 0;
    while (is_symbol_char(s[len]) && s[len] != '\0') {
        len++;
    }

    return len;
}

#define IS_IGNORE_CHAR(c)   (c == ' ' || c == '\t' || c == '\r')
#define IS_PREPROCESS(c)    (c == '@')
#define IS_COMMENT_BEGIN(s) (c == '/' && &(c + 1) == '*')
#define IS_COMMENT_END(s)   (c == '*' && &() == '/')

void tokenize(char *src, VectorI64 *src_tokens) {
    int64_t i = 0;
    while (src[i] != 0) {
        if (IS_IGNORE_CHAR(src[i])) {
            i++;
            continue;
        
        } else if (src[i] == '/' && src[i + 1] == '*') {
            while (!(src[i] == '*' && src[i + 1] == '/')) {
                i++;
            }
            i += 2;
            continue;
        }

        size_t len = 0;
        tokentype_t type = -1;
        if (IS_PREPROCESS(src[i])) {
            // pre-process
            preprocess(src, i, src_tokens);
            while (src[i] != '\n' && src[i] != '\0') {
                i++;
            }

            continue;
        
        } else if (src[i] == '\\') {
            // 1行に書かなければいけない処理を複数行書けるようにする
            do {
                i++;
            } while (src[i] == ' ' || src[i] == '\n');
            continue;
        
        } else if (src[i] == ';') {
            // 改行と読み替える
            src[i] = '\n';
            len  = 1;
            type = TK_TY_SYMBOL;
        
        } else if (strchr("()[]:,\n", src[i]) != 0) {
            len  = 1;
            type = TK_TY_SYMBOL;

        } else if (is_symbol_char(src[i])) {
            len = count_operator_len(&src[i]);
            if (is_valid_operator(&src[i], len) == false) {

                // call error
            }
            type = TK_TY_SYMBOL;

        } else if (is_number(src[i])) {
            len  = count_constant_len(&src[i]);
            type = TK_TY_LITERAL;
        
        } else if (is_varname(src[i])) {
            len  = count_varname_len(&src[i]);
            type = TK_TY_VARIABLE;

        } else {
            // エラー処理
            printf("error\n");
            return;
        }

        if (type == -1) {
            // 一応...
            printf("error\n");
            return;
        }

        vector_i64_append(
            src_tokens,
            allocate_tc(&src[i], len, type)
        );
        i += len;
    }

    return;
}

VectorI64 *lexer(char *src) {
    VectorI64 *src_tokens = new_vector_i64(DEFAULT_MAX_TC);
    if (IS_NULL(src_tokens)) {
        // TODO: まじめにかく
        printf("error");
        return NULL;
    }

    tokenize(src, src_tokens);
    vector_i64_append(src_tokens, TC_LF);

    return src_tokens;
}
