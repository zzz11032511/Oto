#include "compiler.h"

SliceI64 *make_line_tokencodes(SliceI64 *srctcs, int64_t start) {
    int64_t end = start;
    while (srctcs->data[end] != TC_LF) {
        end++;
        if (end >= srctcs->length) {
            error_compiler(OTO_INVALID_SYNTAX_ERROR, srctcs, start);
        }
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    return slice;
}

SliceI64 *make_args_enclosed_br(SliceI64 *srctcs, int64_t sqbropn) {
    int64_t start = sqbropn + 1;
    int64_t end = sqbropn;

    int64_t nest = 0;
    for (;;) {
        tokencode_t tc = srctcs->data[end];

        if (tc == TC_SQBROPN || tc == TC_BROPN) {
            nest++;
        } else if  (tc == TC_SQBRCLS || tc == TC_BRCLS) {
            nest--;
        }
        
        if ((tc == TC_SQBRCLS || TC_BRCLS) && nest == 0) {
            break;
        } else if (end >= srctcs->length) {
            error_compiler(OTO_INVALID_SYNTAX_ERROR, srctcs, start);
        }

        end++;
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    return slice;
}

SliceI64 *make_begin_end_block(SliceI64 *srctcs, int64_t begin) {
    int64_t start = begin + 1;
    int64_t end = begin + 1;

    int64_t nest = 1;
    for (;;) {
        tokencode_t tc = srctcs->data[end];

        /* ifの個数とifブロックのendの個数は一致する */
        if (tc == TC_BEGIN || tc == TC_IF) {
            nest++;
        } else if  (tc == TC_END) {
            nest--;
        }

        if (tc == TC_END && nest == 0) {
            break;
        } else if (end >= srctcs->length) {
            error_compiler(OTO_INVALID_SYNTAX_ERROR, srctcs, srctcs->abs_idx + start);
        }

        end++;
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    return slice;
}

/* then ~ elsif, elseのばしょを返す */
SliceI64 *make_ifthen_block(SliceI64 *srctcs, int64_t then) {
    int64_t start = then + 1;
    int64_t end = then;

    for (;;) {
        tokencode_t tc = srctcs->data[end];

        // ブロック内のif-end, begin-endを飛ばす
        if (tc == TC_IF || tc == TC_BEGIN) {
            int64_t nest = 0;
            for (;;) {
                tc = srctcs->data[end];
                if (tc == TC_IF || tc == TC_BEGIN) {
                    nest++;
                } else if (tc == TC_END) {
                    nest--;
                }

                if (tc == TC_END && nest == 0) {
                    break;
                } else if (end >= srctcs->length) {
                    error_compiler(OTO_INVALID_SYNTAX_ERROR, srctcs, start);
                }  
                end++;
            }
        } else if (tc == TC_ELSIF || tc == TC_ELSE || tc == TC_END) {
            break;
        } else if (end >= srctcs->length) {
            error_compiler(OTO_INVALID_SYNTAX_ERROR, srctcs, start);
        }

        end++;
    }

    SliceI64 *slice = new_slice_i64_from_slice(srctcs, start, end);
    if (IS_NULL(slice)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    return slice;
}

void compile_args(int64_t *icp, SliceI64 *argtcs, int64_t max_params) {
    int64_t idx = 0;
    int64_t params = 0;
    tokencode_t tc = slice_i64_get(argtcs, idx);

    while (tc != TC_LF) {
        int64_t start = idx;
        int64_t end   = idx;
        while (slice_i64_get(argtcs, end) != TC_COMMA 
               && slice_i64_get(argtcs, end) != TC_LF) {
            end++;
        }

        if (end - start == 1) {
            put_opcode(icp, OP_PUSH, VAR(tc), 0, 0, 0);
            idx += 1;

        } else if (end - start == 0) {
            put_opcode(icp, OP_PUSH_INITVAL, 0, 0, 0, 0);

        } else {
            // 引数一つのコンパイル
            SliceI64 *slice = new_slice_i64_from_slice(argtcs, start, end);
            compile_expr(icp, slice, vars);
            idx += slice->length;
            free_slice_i64(slice);          
        }

        params++;
        idx++;
        tc = slice_i64_get(argtcs, idx);
    }

    if (params > max_params) {
        error_compiler(OTO_TOO_MANY_ARGUMENTS_ERROR, argtcs, 0);
    }

    while (params < max_params) {
        put_opcode(icp, OP_PUSH_INITVAL, 0, 0, 0, 0);
        params++;
    }
}

void print_slice_srcs(SliceI64 *srctcs) {
    printf("\n[slice]\n");

    for (int64_t i = 0; i < srctcs->length; i++) {
        printf("%I64d : %s\n", i, VAR(slice_i64_get(srctcs, i))->token->str);
    }
    printf("\n");
}

void error_compiler(errorcode_t err, SliceI64 *srctcs, int64_t idx) {
    print_error(err, oto_status);

    int64_t srcidx = tc2srcidx(srctcs, srctcs->abs_idx + idx);
    // printf("srcidx : %I64d\n", srcidx);

    int64_t line = get_current_line(src, srcidx);
    printf("line %I64d : ", line);
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

    // "line OOO : "の分だけ空白をあける
    printf("        ");
    int j = 0;
    while (j <= (line / 10)) {
        printf(" ");
        j++;
    }

    while (i < idx) {
        printf(" ");
        i++;
    }
    fprintf(stderr, "\x1b[31m");
    fprintf(stderr, "^~~");
    fprintf(stderr, "\x1b[39m");
    printf("\n");

    oto_error_throw(err);
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

static size_t count_operator_len(char *s) {
    size_t len = 0;
    while (is_symbol_char(s[len]) && s[len] != '\0') {
        len++;
    }

    return len;
}

#define IS_IGNORE_CHAR(c)   (c == ' ' || c == '\t' || c == '\r')

int64_t tc2srcidx(SliceI64 *srctcs, int64_t tcidx) {
    int64_t tci = 0;
    int64_t i = 0;
    // printf("tcidx : %I64d\n", tcidx);
    while (src[i] != 0) {
        if (tci == tcidx) {
            return i;
        }

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
                    if (nest == 0) break;

                } else if (src[i] == 0) {
                    return 0;
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
        if (src[i] == '@') {
            while (src[i] != '\n' && src[i] != '\0') i++;
            continue;
        
        } else if (src[i] == '\\') {
            do {
                i++;
            } while (src[i] == ' ' || src[i] == '\n');
            continue;
        
        } else if (src[i] == ';') {
            // 改行と読み替える
            src[i] = '\n';
            len  = 1;
        
        } else if (strchr("()[]:,\n", src[i]) != 0) {
            len  = 1;

        } else if (is_symbol_char(src[i])) {
            len = count_operator_len(&src[i]);

        } else if (is_number(src[i])) {
            len  = count_constant_len(&src[i]);
        
        } else if (is_varname(src[i])) {
            len  = count_varname_len(&src[i]);

        }

        tci += 1;
        i += len;
    }
    return 0;
}