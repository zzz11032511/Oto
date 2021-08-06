#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run.h"
#include "error/error.h"
#include "utils/util.h"

/* 使い方表示用の関数 */
void usage(str_t name) {
    fprintf(stderr, "Example : %s XXX.oto\n", name);
    return;
}

int main(int argc, const char **argv) {
    int i = 1;

    if (argc == 1) {
        usage((str_t)argv[0]);
        exit(1);
    }

    /* 実行時間表示 */
    if (argc > 1 && strcmp(argv[1], "-T") == 0) {
        set_timecount_flag(true);
        i++;
    }

    if (argc != i + 1) {
        usage((str_t)argv[0]);
        exit(1);
    }

    init_error();

    run((str_t)argv[i]);

    oto_quit(EXIT_SUCCESS);

    return 0;
}