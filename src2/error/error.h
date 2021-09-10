#pragma once

typedef enum {
    SYNTAX_ERROR = 0,
    TOO_MANY_TOKEN_ERROR,
    FILE_NOT_FOUND_ERROR,
    INCLUDE_FILE_NOT_FOUND_ERROR,
    
    INVALID_SYNTAX_ERROR,
    NAME_ERROR,
    ASSIGN_TO_LITERAL_ERROR,
    DEFINE_ERROR,

    STACK_OVERFLOW_ERROR,
    ZERO_DIVISION_ERROR,
    UNKNOWN_ERROR
} errorcode_t;

void set_filename(const int8_t *filename);
void set_src(int8_t *src);

void oto_quit(int32_t exit_status);

void call_error(errorcode_t error_code);