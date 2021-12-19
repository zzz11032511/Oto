#include <oto.h>

int64_t get_current_line(char *src, int64_t idx) {
    int64_t line = 1;
    
    int64_t i = 0;
    while (i < idx) {
        if (src[i] == '\n') {
            line++;
        } else if (src[i] == '\0') {
            break;
        }
        i++;
    }

    return line;
}

void print_line(char *src, int64_t line) {
    int64_t l = 1;

    int64_t i = 0;
    while (l != line) {
        if (src[i] == '\n') {
            l++;
        } else if (src[i] == '\0') {
            break;
        }
        i++;
    }

    print_upto_char(&src[i], '\n');
    printf("\n");
}

static void error_lexer(errorcode_t err, char *src, int64_t idx) {
    print_error(err);
    int64_t line = get_current_line(src, idx);
    print_line(src, line);

    int64_t l = 1;
    int64_t i = 0;
    while (l != line) {
        if (src[i] == '\n') {
            l++;
        } else if (src[i] == '\0') {
            break;
        }
        i++;
    }

    while (i < idx) {
        printf(" ");
        i++;
    }
    fprintf(stderr, "\x1b[31m");
    fprintf(stderr, "^");
    fprintf(stderr, "\x1b[39m");
    printf("\n");

    exit(EXIT_FAILURE);
}

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
    if (strchr("=+-*/%<>!:~|'$`^", c) != 0) return true;
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
        if (strncmp(s, symbols[i].str, len) == 0) {
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

void tokenize(char *src, VectorI64 *src_tokens, VectorPTR *var_list, Status *status) {
    int64_t i = 0;
    while (src[i] != 0) {
        if (IS_IGNORE_CHAR(src[i])) {
            i++;
            continue;
        
        } else if (src[i] == '/' && src[i + 1] == '*') {
            int64_t nest = 0;
            for (;;) {
                if (src[i] == '/' && src[i + 1] == '*') {
                    nest++;
                } else if (src[i] == '*' && src[i + 1] == '/') {
                    nest--;
                    if (nest == 0) {
                        break;
                    }
                } else if (src[i] == 0) {
                    error_lexer(OTO_SYNTAX_ERROR, src, i);
                }
                i++;
            }
            i += 2;
            continue;

        } else if (src[i] == -29 && src[i + 1] == -128 && src[i + 2] == -128) {
            // UTF-8の全角スペース
            i += 3;
            continue;

        } else if (src[i] == -127 && src[i + 1] == 64) {
            // Shift-JISの全角スペース
            i += 2;
            continue;

        }

        size_t len = 0;
        tokentype_t type = -1;
        if (IS_PREPROCESS(src[i])) {
            if (get_repl_flag() == true) {
                error_lexer(OTO_REPL_ERROR, src, 0);
            } else {
                preprocess(src, i, src_tokens, var_list, status);
            }
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
                error_lexer(OTO_UNAVAILABLE_OPERATOR_ERROR, src, i);
            }
            type = TK_TY_SYMBOL;

        } else if (is_number(src[i])) {
            len  = count_constant_len(&src[i]);
            type = TK_TY_LITERAL;
        
        } else if (is_varname(src[i])) {
            len  = count_varname_len(&src[i]);
            type = TK_TY_VARIABLE;

        } else {
            error_lexer(OTO_SYNTAX_ERROR, src, i);
        }

        vector_i64_append(
            src_tokens,
            allocate_tc(&src[i], len, type, var_list)
        );
        i += len;
    }

    return;
}

void lexer(char *src, VectorI64 *src_tokens, VectorPTR *var_list, Status *status) {
    VectorI64 *src_tokens = new_vector_i64(DEFAULT_MAX_TC);
    if (IS_NULL(src_tokens)) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    tokenize(src, src_tokens, var_list, status);
    vector_i64_append(src_tokens, TC_LF);

    return src_tokens;
}
