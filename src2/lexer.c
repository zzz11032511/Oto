#include "oto.h"

static bool is_number(int8_t c) {
    if ('0' <= c && c <= '9') return true;
    return false;
}

static bool is_varname(int8_t c) {
    if ('0' <= c && c <= '9') return true;
    if ('a' <= c && c <= 'z') return true;
    if ('A' <= c && c <= 'Z') return true;
    if (c == '_') return true;
    if (c == '#') return true;
    return false;
}

static bool is_symbol_char(int8_t c) {
    if (strchr("=+-*/%<>!", c) != 0) return true;
    return false;
}

static size_t count_constant_len(int8_t *s) {
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

static size_t count_varname_len(int8_t *s) {
    size_t len = 0;
    while (is_varname(s[len])) {
        len++;
    }

    return len;
}

static struct {
    int8_t *name;
    tokencode_t tc;
} symbols[] = {
    {"\n", TC_LF      },
    {",",  TC_COMMA   }, {":",  TC_COLON   },
    {"[",  TC_SQBROPN }, {"]",  TC_SQBRCLS },
    {"(",  TC_BROPN   }, {")",  TC_BRCLS   },
    {"<-", TC_LARROW  }, {"->", TC_RARROW  },
    {"=",  TC_EQU     }, {"+",  TC_PLUS    },
    {"-",  TC_MINUS   }, {"*",  TC_ASTER   },
    {"/",  TC_SLASH   }, {"%",  TC_PERCE   },
    {"==", TC_EEQ     }, {"!=", TC_NEQ     },
    {">",  TC_GT      }, {"<",  TC_LT      },
    {">=", TC_GE      }, {"<=", TC_LE      },
    {NULL, 0          }  
};

static bool is_valid_operator(int8_t *s, size_t len) {
    uint64_t i = 0;
    while (symbols[i].name != NULL) {
        if (strcmp(s, symbols[i].name) == 0) {
            return true;
        }
        i++;
    }

    return false;
}

static size_t count_operator_len(int8_t *s) {
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

void tokenize(char *s, VectorUI64 *src_tokens) {
    uint32_t i = 0;
    while (s[i] != 0) {
        if (IS_IGNORE_CHAR(s[i])) {
            i++;
            continue;
        
        } else if (s[i] == '/' && s[i + 1] == '*') {
            while (!(s[i] == '*' && s[i + 1] == '/')) {
                i++;
            }
            i += 2;
            continue;
        }


        size_t len = 0;
        tokentype_t type = -1;
        if (IS_PREPROCESS(s[i])) {
            // pre-process
        
        } else if (s[i] == '\\') {
            // 1行に書かなければいけない処理を複数行書けるようにする
            do {
                i++;
            } while (s[i] == ' ' || s[i] == '\n');
            continue;
        
        } else if (s[i] == ';') {
            // 改行と読み替える
            s[i] = '\n';
            len  = 1;
            type = TK_TY_SYMBOL;
        
        } else if (strchr("()[]:,\n", s[i]) != 0) {
            len  = 1;
            type = TK_TY_SYMBOL;

        } else if (is_symbol_char(s[i])) {
            len = count_operator_len(&s[i]);
            if (is_valid_operator(&s[i], len) == false) {

                // call error
            }
            type = TK_TY_SYMBOL;

        } else if (is_number(s[i])) {
            len  = count_constant_len(&s[i]);
            type = TK_TY_LITERAL;
        
        } else if (is_varname(s[i])) {
            len  = count_varname_len(&s[i]);
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

        append_vector_ui64(
            src_tokens,
            allocate_tc(&s[i], len, type)
        );
        i += len;
    }

    append_vector_ui64(src_tokens, TC_LF);

    return;
}

void lexer(char *s, VectorUI64 *src_tokens, VectorPTR *var_list) {
    if (IS_NOT_NULL(src_tokens)) {
        free_vector_ui64(src_tokens);
    }

    src_tokens = new_vector_ui64(DEFAULT_MAX_TC);
    if (IS_NULL(src_tokens)) {
        // TODO: まじめにかく
        printf("error");
        return;
    }

    tokenize(s, src_tokens);

    var_list = make_var_list();
    if (IS_NULL(var_list)) {
        // TODO: まじめにかく
        printf("error");
        return;
    }

    return;
}