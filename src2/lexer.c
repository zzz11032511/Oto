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

static bool is_symbol(int8_t c) {
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
    {"\n", TcLF      },
    {",",  TcComma   }, {":",  TcColon   },
    {"[",  TcSqBrOpn }, {"]",  TcSqBrCls },
    {"(",  TcBrOpn   }, {")",  TcBrCls   },
    {"<-", TcLArrow  }, {"->", TcRArrow  },
    {"=",  TcEqu     }, {"+",  TcPlus    },
    {"-",  TcMinus   }, {"*",  TcAster   },
    {"/",  TcSlash   }, {"%",  TcPerce   },
    {"==", TcEEq     }, {"!=", TcNEq     },
    {">",  TcGt      }, {"<",  TcLt      },
    {">=", TcGe      }, {"<=", TcLe      },
    {NULL, 0         }  
};

static bool is_valid_operator(int8_t *s, size_t len) {
    uint64_t i = 0;
    while (symbols[i].name != NULL) {
        if (strcmp(s, symbols[i].name) == 0) {
            return true;
        }
    }

    return false;
}

static size_t count_operator_len(int8_t *s) {
    size_t len = 0;
    while (is_symbol(s[len]) && s[len] != '\0') {
        len++;
    }

    return len;
}

#define IS_IGNORE_CHAR(c) c == ' ' || c == '\t' || c == '\r'
#define IS_PREPROCESS(c) c == '@'

uint32_t lexer(int8_t *s) {
    
}