#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "lexer.h"
#include "../util/util.h"
#include "../util/linked_list.h"
#include "../error/error.h"
#include "../token/token.h"
#include "../variable/type.h"

typedef enum {
    cmdNone = 0,
    cmdInclude
} pre_command_t;

/* どのプリプロセッサ処理かを調べる */
pre_command_t search_command(int8_t *cmd_start, size_t len) {
    if (strncmp(cmd_start, "include", len) ||
        strncmp(cmd_start, "INCLUDE", len)) {
        return cmdInclude;
    } else {
        return cmdNone;
    }
}

/* 循環参照エラーを防ぐために現在参照しているファイルをリスト化する */
static struct str_list *include_src_list = NULL;

void include_otofile(int8_t *arg_start, size_t fname_len) {
    int8_t fname[fname_len + 5];

    strncpy(fname, arg_start, fname_len);

    if (strstr(fname, ".oto") == NULL) {
        fname[fname_len + 0] = '.';
        fname[fname_len + 1] = 'o';
        fname[fname_len + 2] = 't';
        fname[fname_len + 3] = 'o';
        fname[fname_len + 4] = '\0';        
    } else {
        fname[fname_len + 0] = '\0';
    }

#ifdef DEBUG
    printf("include %s\n", fname);
#endif

    // 循環参照の検知
    // TODO: ./の有無で多少無駄になる
    if (include_src_list == NULL) {
        include_src_list = new_str_list();
        if (include_src_list == NULL) {
            call_error(UNKNOWN_ERROR);
        }
        append_str_item(include_src_list, get_filename());
    } else if (is_exist_str_item(include_src_list, fname)) {
        call_error(CIRCULAR_REFERENCE_ERROR);
    }

    append_str_item(include_src_list, fname);

    int8_t *src = src_open(fname);
    if (src == NULL) {
        call_error(INCLUDE_FILE_NOT_FOUND_ERROR, fname);
    }

    lexer(src);

    // 無事にincludeしたファイルをリストから削除
    remove_tail_str_item(include_src_list);
    free(src);
    if (count_str_num_of_item(include_src_list) <= 0) {
        free_str_list(include_src_list);
    }
}

uint32_t preprocess(int8_t *pre_start) {
    uint32_t i = 1;
    int8_t *cmd_start = &pre_start[1];

    size_t cmd_len = count_string_length(cmd_start, ' ');
    i += cmd_len;

    // 空白を飛ばす
    while (pre_start[i] == ' ') {
        if (pre_start[i] == '\n' || pre_start[i] == '\0') {
            break;
        }
        i++;
    }

    size_t arg_len = 0;
    int8_t *arg_start = &pre_start[i];

    pre_command_t cmd = search_command(cmd_start, cmd_len);
    switch (cmd) {
    case cmdInclude:
        arg_len = count_string_length(arg_start, '\n');
        if (arg_len != 0) {
            include_otofile(arg_start, arg_len);
        }
        i += arg_len;
        break;

    case cmdNone:
        while (pre_start[i] != '\n' && pre_start[i] != '\0') i++;
        break;
    }

    return i;
}