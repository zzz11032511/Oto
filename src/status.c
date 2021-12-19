#include <oto.h>

static Status oto_status = {
    false,  // timecount_flag
    false,  // repl_flag
    NULL,   // root_srcpath
    NULL,   // srcfile_table
    LANG_JPN_KANJI,  // language
    44100   // sampling_rate
};

Status *get_oto_status() {
    return &oto_status;
}

static void push_conf_table(Map *conf_table, char *param, size_t paramlen,
                            char *value, size_t valuelen) {    
    char *key = MYMALLOC(paramlen + 1, char);
    if (IS_NULL(key)) {
        // error
        return;
    }
    char *val = MYMALLOC(valuelen + 1, char);
    if (IS_NULL(val)) {
        // error
        return;
    }

    strncpy(key, param, paramlen);
    strncpy(val, value, valuelen);
    
    map_put(conf_table, key, val);
}

static Map *parse_otoconf(char *conf) {
    Map *conf_table = new_map();
    if (IS_NULL(conf_table)) {
        return NULL;
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

        push_conf_table(conf_table, param, paramlen, value, valuelen);
        i += valuelen + 1;
    }

    return conf_table;
}

static void load_config(Status *status, Map *conf_table) {
    char *option;
    if (map_exist_key(conf_table, "timecount")) {
        option = map_get(conf_table, "timecount");
        if (strcmp(option, "true") == 0) {
            status->timecount_flag = true;
        } else if (strcmp(option, "false") == 0) {
            status->timecount_flag = false;
        }
    }

    if (map_exist_key(conf_table, "repl")) {
        option = map_get(conf_table, "repl");
        if (strcmp(option, "true") == 0) {
            status->repl_flag = true;
        } else if (strcmp(option, "false") == 0) {
            status->repl_flag = false;
        }
    }

    if (map_exist_key(conf_table, "language")) {
        option = map_get(conf_table, "language");
        if (strcmp(option, "kanji") == 0) {
            set_language(LANG_JPN_KANJI);
        } else if (strcmp(option, "hiragana") == 0) {
            set_language(LANG_JPN_HIRAGANA);
        } else if (strcmp(option, "english") == 0) {
            set_language(LANG_ENG);
        }
    }

    if (map_exist_key(conf_table, "sampling_rate")) {
        option = map_get(conf_table, "language");
        status->sampling_rate = strtol(option, NULL, 0);
    }

    if (map_exist_key(conf_table, "default_srcpath")) {
        status->root_srcpath = map_get(conf_table, "default_srcpath");
        map_puti(status->srcfile_table, status->root_srcpath, 1);
    }
}

const char otoconf_path[] = ".otoconf";
void init_status(Status *status, char *srcpath) {
    char *conf = src_open(otoconf_path);
    if (IS_NULL(conf)) {
        return;
    }

    Map *conf_table = parse_otoconf(conf);
    if (conf_table == NULL) {
        return;
    }

    status->srcfile_table = new_map();
    if (conf_table == NULL) {
        return;
    }

    load_config(status, conf_table);

    if (IS_NOT_NULL(srcpath)) {
        status->root_srcpath = srcpath;
        map_puti(status->srcfile_table, srcpath, 1);
    }

    free(conf_table);
    free(conf);
}
