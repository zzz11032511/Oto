#include <oto/oto.h>

void print_error(errorcode_t err, Status *status) {
    fprintf(stderr, "\x1b[31m");
    fprintf(stderr, "ERROR : ");
    fprintf(stderr, "\x1b[39m");

    switch (err) {
    case OTO_NO_ERROR:
        break;

    case OTO_UNKNOWN_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "正体不明のエラー\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "エラー\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Unknown error\n");
        }
        break;

    case OTO_INTERNAL_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "内部エラー\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "エラー\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Internal error\n");
        }
        break;

    case OTO_SYNTAX_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "書き方が間違っています\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "かきかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Syntax error\n");
        }
        break;

    case OTO_PREPROCESS_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "プリプロセッサのエラーです\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "かきかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Preprocess error\n");
        }
        break;

    case OTO_FILE_NOT_FOUND_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "ファイルがありません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "ファイルが ありません\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "File not found\n");
        }
        break;

    case OTO_INCLUDE_FILE_NOT_FOUND_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "インクルードするファイルが存在していません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "ファイルが ありません\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Include file not found error\n");
        }
        break;

    case OTO_CIRCULAR_REFERENCE_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "循環参照が発生しています\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "インクルードの やりかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Circular reference error\n");
        }
        break;

    case OTO_UNAVAILABLE_OPERATOR_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "使用できない演算子です\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "きごうが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Unavailable operator error\n");
        }
        break;

    case OTO_INVALID_SYNTAX_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "構文エラーです\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "かきかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Invalid syntax error\n");
        }
        break;

    case OTO_DEFINE_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "DEFINE文で変数を使うことはできません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "DEFINEの つかいかたが ちがいます\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Define error\n");
        }
        break;

    case OTO_NAME_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "変数に使用できない名前です\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "へんすうに つかえない なまえです\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Name error\n");
        }
        break;

    case OTO_ASSIGN_TO_LITERAL_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "定数に代入することはできません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "すうじに なにかいれることは できません\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Assign to literal error\n");
        }
        break;

    case OTO_TOO_MANY_ARGUMENTS_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "命令の引数が多すぎます\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "めいれいの ひきすうが おおすぎます\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Too many arguments error\n");
        }
        break;

    case OTO_MISSING_ARGUMENTS_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "命令の引数が足りません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "めいれいの ひきすうが たりません\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Missing arguments error\n");
        }
        break;

    case OTO_STACK_OVERFLOW_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "スタックオーバーフロー\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "エラー");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Stack overflow error");
        }
        break;

    case OTO_ZERO_DIVISION_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "0で割ることはできません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "0で わっては いけません\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Zero division error\n");
        }
        break;

    case OTO_SOUND_PLAYER_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "音声出力に失敗しました\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "おとが うまく でませんでした\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "Oto sound player error\n");
        }
        break;
    
    case OTO_REPL_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            fprintf(stderr, "REPLモードでは実行できない命令です\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            fprintf(stderr, "このモードでは つかえない めいれいです\n");
        } else if (status->language == LANG_ENG) {
            fprintf(stderr, "This instruction cannot be used in REPL mode\n");
        }
        break;

    default:
        break;
    }
}
