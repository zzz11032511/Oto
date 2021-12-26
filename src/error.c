#include <oto/oto.h>

void print_error(errorcode_t err, Status *status) {
    printf("\x1b[31m");
    printf("ERROR");
#ifdef DEBUG
    printf("(%I64d) : ", err + 10001);
    printf("\x1b[39m");
#else
    printf("\x1b[39m");
    printf(" : ");
#endif

    switch (err) {
    case OTO_NO_ERROR:
        break;

    case OTO_UNKNOWN_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("正体不明のエラー\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("エラー\n");
        } else if (status->language == LANG_ENG) {
            printf("Unknown error\n");
        }
        break;

    case OTO_INTERNAL_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("内部エラー\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("エラー\n");
        } else if (status->language == LANG_ENG) {
            printf("Internal error\n");
        }
        break;

    case OTO_SYNTAX_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("書き方が間違っています\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("かきかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            printf("Syntax error\n");
        }
        break;

    case OTO_PREPROCESS_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("プリプロセッサのエラーです\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("かきかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            printf("Preprocess error\n");
        }
        break;

    case OTO_FILE_NOT_FOUND_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("ファイルがありません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("ファイルが ありません\n");
        } else if (status->language == LANG_ENG) {
            printf("File not found\n");
        }
        break;

    case OTO_INCLUDE_FILE_NOT_FOUND_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("インクルードするファイルが存在していません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("ファイルが ありません\n");
        } else if (status->language == LANG_ENG) {
            printf("Include file not found error\n");
        }
        break;

    case OTO_CIRCULAR_REFERENCE_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("循環参照が発生しています\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("インクルードの やりかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            printf("Circular reference error\n");
        }
        break;

    case OTO_UNAVAILABLE_OPERATOR_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("使用できない演算子です\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("きごうが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            printf("Unavailable operator error\n");
        }
        break;

    case OTO_INVALID_SYNTAX_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("構文エラーです\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("かきかたが まちがえています\n");
        } else if (status->language == LANG_ENG) {
            printf("Invalid syntax error\n");
        }
        break;

    case OTO_DEFINE_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("DEFINE文で変数を使うことはできません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("DEFINEの つかいかたが ちがいます\n");
        } else if (status->language == LANG_ENG) {
            printf("Define error\n");
        }
        break;

    case OTO_NAME_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("変数に使用できない名前です\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("へんすうに つかえない なまえです\n");
        } else if (status->language == LANG_ENG) {
            printf("Name error\n");
        }
        break;

    case OTO_ASSIGN_TO_LITERAL_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("定数に代入することはできません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("すうじに なにかいれることは できません\n");
        } else if (status->language == LANG_ENG) {
            printf("Assign to literal error\n");
        }
        break;

    case OTO_TOO_MANY_ARGUMENTS_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("命令の引数が多すぎます\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("めいれいの ひきすうが おおすぎます\n");
        } else if (status->language == LANG_ENG) {
            printf("Too many arguments error\n");
        }
        break;

    case OTO_MISSING_ARGUMENTS_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("命令の引数が足りません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("めいれいの ひきすうが たりません\n");
        } else if (status->language == LANG_ENG) {
            printf("Missing arguments error\n");
        }
        break;

    case OTO_STACK_OVERFLOW_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("スタックオーバーフロー\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("エラー");
        } else if (status->language == LANG_ENG) {
            printf("Stack overflow error");
        }
        break;

    case OTO_ZERO_DIVISION_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("0で割ることはできません\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("0で わっては いけません\n");
        } else if (status->language == LANG_ENG) {
            printf("Zero division error\n");
        }
        break;

    case OTO_SOUND_PLAYER_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("音声出力に失敗しました\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("おとが うまく でませんでした\n");
        } else if (status->language == LANG_ENG) {
            printf("Oto sound player error\n");
        }
        break;
    
    case OTO_REPL_ERROR:
        if (status->language == LANG_JPN_KANJI) {
            printf("REPLモードでは実行できない命令です\n");
        } else if (status->language == LANG_JPN_HIRAGANA) {
            printf("このモードでは つかえない めいれいです\n");
        } else if (status->language == LANG_ENG) {
            printf("This instruction cannot be used in REPL mode\n");
        }
        break;

    default:
        break;
    }
}
