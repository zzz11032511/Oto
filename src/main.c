#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run.h"
#include "errorHandle.h"
#include "utils/util.h"

/* 使い方表示用の関数 */
void usage(str_t name) {
    // TODO: そのうち真面目に書く
    fprintf(stderr, "Example : %s XXX.oto\n", name);
    return;
}

int main(int argc, const char **argv) {
    if (argc <= 1) {
        usage((str_t)argv[0]);
        exit(1);
    }

    initErrorHandle();

    str_t src = NULL;

    setFname((str_t)argv[1]);

    if (srcLoad((str_t)argv[1], &src) == 0) {
        setSrc(src);
        run(src);
    }

    otoQuit(EXIT_SUCCESS);

    return 0;
}