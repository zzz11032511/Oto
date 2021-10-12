#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"
#include "../util/util.h"
#include "../token/token.h"
#include "../variable/type.h"

static int8_t *e_filename = NULL;
static int8_t *e_src = NULL;

void set_filename(const int8_t *filename) {
    size_t len = count_string_length(filename, '\0');
    e_filename = calloc(1, len);
    strncpy(e_filename, filename, len);
}

void set_src(int8_t *src) {
    e_src = src;
}

int8_t *get_filename() {
    return e_filename;
}

void oto_quit(int32_t exit_status) {
    free(e_src);
    free(e_filename);
    free_var();
    e_src = NULL;
    exit(exit_status);
}

/* ソースコード上からエラーが発生した行を特定する */
uint32_t find_error_line_str(uint32_t error_idx) {
    int32_t num_of_lines = 1;

    uint32_t i = 0;
    while (i < error_idx) {
        if (e_src[i] == '\n') num_of_lines++;
        i++;
    }

    return num_of_lines;
}

/* ソースコードから指定した行の先頭を特定する */
uint32_t find_line_head(uint32_t num_of_lines) {
    int32_t head = 0;
    int32_t line = 1;

    uint32_t i = 0;
    while (line != num_of_lines) {
        if (e_src[i] == '\n') {
            head = i + 1;
            line++;
        }
        i++;
    }

    return head;
}

void print_error_line(uint32_t i) {
    uint32_t line = find_error_line_str(i);
    uint32_t head = find_line_head(line);
    printf("line %d : ", line);
    line_print(&e_src[head]);
}

void call_error(errorcode_t error_code, ...) {
    va_list ap;
    va_start(ap, error_code);

    fprintf(stderr, "Error [%s]\n", e_filename);
    switch (error_code) {
    case SYNTAX_ERROR:
        printf("Syntax error\n");
        print_error_line(va_arg(ap, uint32_t));
        break;

    case INVALID_SYNTAX_ERROR:
        fprintf(stderr, "Invalid syntax\n");
        break;

    case UNAVAILABLE_OPERATORS_ERROR:
        printf("Operators cannot be used\n");
        print_error_line(va_arg(ap, uint32_t));
        break;

    case FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "File not found\n");
        printf("filename : %s\n", va_arg(ap, int8_t *));
        break;

    case INCLUDE_FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "Include file not found\n");
        printf("filename : %s\n", va_arg(ap, int8_t *));
        break;

    case CIRCULAR_REFERENCE_ERROR:
        fprintf(stderr, "Circular reference\n");
        break;

    case NAME_ERROR:
        fprintf(stderr, "Cannot use reserved words for variable name\n");
        break;

    case ASSIGN_TO_LITERAL_ERROR:
        fprintf(stderr, "Cannot assign to literal\n");
        break;

    case DEFINE_ERROR:
        fprintf(stderr, "Cannot define non-constant\n");
        break;

    case TOO_MANY_ARGUMENTS_ERROR:
        fprintf(stderr, "Too many arguments\n");
        break;

    case ZERO_DIVISION_ERROR:
        fprintf(stderr, "Division by zero\n");
        break;

    case SOUND_PLAYER_ERROR:
        fprintf(stderr, "Sound player error\n");
        break;

    case UNKNOWN_ERROR:
        fprintf(stderr, "Unknown error\n");
        printf("Location : %s\n", va_arg(ap, int8_t *));
        break;

    default:
        fprintf(stderr, "errorcode(%d)\n", error_code);
        break;
    }

    va_end(ap);

    oto_quit(EXIT_FAILURE);
}