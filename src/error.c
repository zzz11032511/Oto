#include <oto.h>

void print_error(errorcode_t err) {
    fprintf(stderr, "\x1b[31m");
    fprintf(stderr, "ERROR : ");
    fprintf(stderr, "\x1b[39m");

    switch (err) {
    case OTO_NO_ERROR:
        break;

    case OTO_UNKNOWN_ERROR:
    case OTO_INTERNAL_ERROR:
        fprintf(stderr, "Unknown error\n");
        break;

    case OTO_SYNTAX_ERROR:
        fprintf(stderr, "Syntax error\n");
        break;

    case OTO_PREPROCESS_ERROR:
        fprintf(stderr, "Preprocess error\n");
        break;

    case OTO_FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "File not found error\n");
        break;

    case OTO_INCLUDE_FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "Include file not found error\n");
        break;

    case OTO_CIRCULAR_REFERENCE_ERROR:
        fprintf(stderr, "Circular reference error\n");
        break;

    case OTO_UNAVAILABLE_OPERATOR_ERROR:
        fprintf(stderr, "Unavailable operator error\n");
        break;

    case OTO_INVALID_SYNTAX_ERROR:
        fprintf(stderr, "Invalid syntax error\n");
        break;

    case OTO_DEFINE_ERROR:
        fprintf(stderr, "Define error\n");
        break;

    case OTO_NAME_ERROR:
        fprintf(stderr, "Name error\n");
        break;

    case OTO_ASSIGN_TO_LITERAL_ERROR:
        fprintf(stderr, "Assign to literal error\n");
        break;

    case OTO_TOO_MANY_ARGUMENTS_ERROR:
        fprintf(stderr, "Too many arguments error\n");
        break;

    case OTO_MISSING_ARGUMENTS_ERROR:
        fprintf(stderr, "Missing arguments error\n");
        break;

    case OTO_STACK_OVERFLOW_ERROR:
        fprintf(stderr, "Stack overflow error\n");
        break;

    case OTO_ZERO_DIVISION_ERROR:
        fprintf(stderr, "Zero division error\n");
        break;

    case OTO_SOUND_PLAYER_ERROR:
        fprintf(stderr, "Sound player error\n");
        break;
    
    case OTO_REPL_ERROR:
        fprintf(stderr, "REPLモードでは実行できない命令です\n");
        break;

    default:
        break;
    }
}
