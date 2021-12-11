#include <oto.h>

void print_error(errorcode_t err) {
    fprintf(stderr, "\x1b[31m");
    fprintf(stderr, "ERROR : ");
    fprintf(stderr, "\x1b[39m");

    switch (err) {
    case OTO_NO_ERROR:
        break;

    case OTO_UNKNOWN_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "正体不明のエラー\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "エラー\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Unknown error\n");
        }
        break;

    case OTO_INTERNAL_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "内部エラー\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "エラー\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Internal error\n");
        }
        break;

    case OTO_SYNTAX_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "書き方が間違っています\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "かきかたがまちがえています\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Syntax error\n");
        }
        break;

    case OTO_PREPROCESS_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "プリプロセッサのエラーです\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "かきかたがまちがえています\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Preprocess error\n");
        }
        break;

    case OTO_FILE_NOT_FOUND_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "ファイルがありません\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "ファイルがありません\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "File not found\n");
        }
        break;

    case OTO_INCLUDE_FILE_NOT_FOUND_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "読み込むファイルがありません\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "ファイルがありません\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Include file not found error\n");
        }
        break;

    case OTO_CIRCULAR_REFERENCE_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "循環参照が発生しています\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "インクルードのやりかたがまちがえています\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Circular reference error\n");
        }
        break;

    case OTO_UNAVAILABLE_OPERATOR_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "使用できない演算子です\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "きごうがまちがえています\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Unavailable operator error\n");
        }
        break;

    case OTO_INVALID_SYNTAX_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "書き方が間違っています\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "かきかたがまちがえています\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Invalid syntax error\n");
        }
        break;

    case OTO_DEFINE_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "DEFINEの使い方がちがいます\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "DEFINEのつかいかたがちがいます\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Define error\n");
        }
        break;

    case OTO_NAME_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "変数に使用できない名前です\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "へんすうにつかえないなまえです\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Name error\n");
        }
        break;

    case OTO_ASSIGN_TO_LITERAL_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "定数に代入することはできません\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "すうじになにかいれることはできません\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Assign to literal error\n");
        }
        break;

    case OTO_TOO_MANY_ARGUMENTS_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "命令の引数が多すぎます\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "めいれいのひきすうがおおすぎます\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Too many arguments error\n");
        }
        break;

    case OTO_MISSING_ARGUMENTS_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "命令の引数が足りません\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "めいれいのひきすうがたりません\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Missing arguments error\n");
        }
        break;

    case OTO_STACK_OVERFLOW_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "スタックオーバーフロー\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "エラー");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Stack overflow error");
        }
        break;

    case OTO_ZERO_DIVISION_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "0で割ることはできません\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "0でわってはいけません\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Zero division error\n");
        }
        break;

    case OTO_SOUND_PLAYER_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "音声出力に失敗しました\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "おとがうまくでませんでした\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "Oto sound player error\n");
        }
        break;
    
    case OTO_REPL_ERROR:
        if (get_language() == LANG_JPN_KANJI) {
            fprintf(stderr, "REPLモードでは実行できない命令です\n");
        } else if (get_language() == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "このモードではつかえないめいれいです\n");
        } else if (get_language() == LANG_ENG) {
            fprintf(stderr, "This instruction cannot be used in REPL mode\n");
        }
        break;

    default:
        break;
    }
}
