#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "error.h"
#include "../util/util.h"
#include "../token/token.h"
#include "../variable/type.h"

static const int8_t *e_filename = NULL;
static int8_t *e_src = NULL;

void set_filename(const int8_t *filename) {
    e_filename = filename;
}

void set_src(int8_t *src) {
    e_src = src;
}

void oto_quit(int32_t exit_status) {
    free(e_src);
    e_src = NULL;
    exit(exit_status);
}

void call_error(errorcode_t error_code) {
    fprintf(stderr, "Error [%s]\n", e_filename);
    switch (error_code) {
    case FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "File not found\n");
        break;
    case INCLUDE_FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "Include file not found\n");
    default:
        fprintf(stderr, "errorcode(%d)\n", error_code);
        break;
    }

    oto_quit(EXIT_FAILURE);
}