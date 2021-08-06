#include <stdint.h>
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
    if (argc <= 1) {
        usage((str_t)argv[0]);
        exit(1);
    }

    init_error();

    if (run((str_t)argv[1])) {
        oto_quit(EXIT_FAILURE);
    }

    oto_quit(EXIT_SUCCESS);

    return 0;
}