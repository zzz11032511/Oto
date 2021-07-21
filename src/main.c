#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "run.h"
#include "errorHandle.h"
#include "utils/util.h"

// 1のときにデバッグのための情報が出力される

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
    
    // ソースファイルを確保しておく領域
    str_t src = NULL;

    // エラー出力用
    setFname((str_t)argv[1]);

    if (srcLoad((str_t)argv[1], &src) == 0) {
        setSrc(src);
        run(src);
    }
    
    free(src);

    return 0;
}