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
        fprintf(stderr, "正体不明のエラー\n");
        break;

    case OTO_SYNTAX_ERROR:
        fprintf(stderr, "書き方が間違っています\n");
        break;

    case OTO_PREPROCESS_ERROR:
        fprintf(stderr, "プリプロセッサのエラーです\n");
        break;

    case OTO_FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "ファイルがありません\n");
        break;

    case OTO_INCLUDE_FILE_NOT_FOUND_ERROR:
        fprintf(stderr, "読み込むファイルがありません\n");
        break;

    case OTO_CIRCULAR_REFERENCE_ERROR:
        fprintf(stderr, "循環参照が発生しています\n");
        break;

    case OTO_UNAVAILABLE_OPERATOR_ERROR:
        fprintf(stderr, "使用できない演算子です\n");
        break;

    case OTO_INVALID_SYNTAX_ERROR:
        fprintf(stderr, "書き方が間違っています\n");
        break;

    case OTO_DEFINE_ERROR:
        fprintf(stderr, "DEFINEの使い方がちがいます\n");
        break;

    case OTO_NAME_ERROR:
        fprintf(stderr, "変数に使用できない名前です\n");
        break;

    case OTO_ASSIGN_TO_LITERAL_ERROR:
        fprintf(stderr, "定数に代入することはできません\n");
        break;

    case OTO_TOO_MANY_ARGUMENTS_ERROR:
        fprintf(stderr, "命令の引数が多すぎます\n");
        break;

    case OTO_MISSING_ARGUMENTS_ERROR:
        fprintf(stderr, "命令の引数が足りません\n");
        break;

    case OTO_STACK_OVERFLOW_ERROR:
        fprintf(stderr, "スタックオーバーフロー\n");
        break;

    case OTO_ZERO_DIVISION_ERROR:
        fprintf(stderr, "0で割ることはできません\n");
        break;

    case OTO_SOUND_PLAYER_ERROR:
        fprintf(stderr, "音声出力がうまくできませんでした\n");
        break;
    
    case OTO_REPL_ERROR:
        fprintf(stderr, "REPLモードでは実行できない命令です\n");
        break;

    default:
        break;
    }
}
