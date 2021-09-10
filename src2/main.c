#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "run.h"
#include "error/error.h"

void usage(const char *name) {
    fprintf(stderr, "Example : %s XXX.oto\n", name);
    return;
}

int main(int argc, const char **argv) {
    uint32_t i = 1;

    if (argc == 1) {
        usage(argv[0]);
        exit(0);
    }

    // 実行時間表示
    if (argc > 1 && strcmp(argv[1], "-T") == 0) {
        set_timecount_flag(true);
        i++;
    }

    // ファイル名が指定されていない場合
    if (argc != i + 1) {
        usage(argv[0]);
        exit(1);
    }

    set_filename(argv[i]);
    run(argv[i]);

    oto_quit(EXIT_SUCCESS);

    return 0;
}