#include <oto.h>

const char otoconf_path[] = ".otoconf";

void load_config() {
    char *conf = src_open(otoconf_path);
    if (IS_NULL(conf)) {
        return;
    }

    int64_t i = 0;
    while (conf[i] != '\0') {
        char *param = &conf[i];
        size_t paramlen = 0;
        while (conf[i + paramlen] != ':' && conf[i + paramlen] != ' '
               && conf[i + paramlen] != '\n' && conf[i + paramlen] != '\0') {
            paramlen++;
        }

        // 空白を飛ばす
        while (conf[i] != ':') i++;
        i++;
        while (conf[i] == ' ') i++;

        if (conf[i] == '\0') {
            break;
        } else if (conf[i] == '\n') {
            i++;
            continue;
        }
        
        char *value = &conf[i];
        size_t valuelen = 0;
        while (conf[i + valuelen] != '\n' && conf[i + valuelen] != '\0') {
            valuelen++;
        }
        i += valuelen + 1;

        if (strncmp(param, "time", 4) == 0) {
            if (strncmp(value, "true", 4) == 0) {
                set_timecount_flag(true);
            } else if (strncmp(value, "false", 5) == 0) {
                set_timecount_flag(false);
            }

        } else if (strncmp(param, "repl", 4) == 0) {
            if (strncmp(value, "true", 4) == 0) {
                set_repl_flag(true);
            } else if (strncmp(value, "false", 5) == 0) {
                set_repl_flag(false);
            }

        } else if (strncmp(param, "language", 8) == 0) {
            if (strncmp(value, "kanji", 5) == 0) {
                set_language(LANG_JPN_KANJI);
            } else if (strncmp(value, "hiragana", 8) == 0) {
                set_language(LANG_JPN_HIRAGANA);
            } else if (strncmp(value, "english", 7) == 0) {
                set_language(LANG_ENG);
            }
        }
    }

    free(conf);
}
