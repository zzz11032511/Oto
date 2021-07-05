/**
 * Oto - Sound Programming Language
 *
 * author : Furuta Naoki
 *          2021/05/18
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run.h"
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

    // ソースファイルを確保しておく領域
    str_t src = (str_t)malloc(sizeof(char) * SOURCE_BUF_SIZE);

    if (srcLoad((str_t)argv[1], src, SOURCE_BUF_SIZE) == 0) {
        run(src);
    }

    free(src);

    return 0;
}