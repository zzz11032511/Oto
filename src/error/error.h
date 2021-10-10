#pragma once

typedef enum {
    SYNTAX_ERROR = 0,
    UNAVAILABLE_OPERATORS_ERROR,

    TOO_MANY_TOKEN_ERROR,
    FILE_NOT_FOUND_ERROR,
    INCLUDE_FILE_NOT_FOUND_ERROR,
    CIRCULAR_REFERENCE_ERROR,
    
    INVALID_SYNTAX_ERROR,
    NAME_ERROR,
    ASSIGN_TO_LITERAL_ERROR,
    DEFINE_ERROR,
    TOO_MANY_ARGUMENTS_ERROR,

    STACK_OVERFLOW_ERROR,
    ZERO_DIVISION_ERROR,
    SOUND_PLAYER_ERROR,
    UNKNOWN_ERROR
} errorcode_t;

void set_filename(const int8_t *filename);
void set_src(int8_t *src);
int8_t *get_filename();

void oto_quit(int32_t exit_status);

void call_error(errorcode_t error_code, ...);