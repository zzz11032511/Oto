#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "exec.h"

/* 使い方表示用の関数 */
void usage() {
    // TODO: そのうち真面目に書く
    printf("Example : oto XXX.oto\n");
    return;
}

int main(int argc, const char **argv)
{
    if (argc <= 1) {
        usage();
        exit(1);
    }
    
    // ソースファイルを確保しておく領域
    String src = (String)malloc(sizeof(char) * SOURCE_BUF_SIZE);
    
    if (srcLoad((String)argv[1], src, SOURCE_BUF_SIZE) == 0) {
        run(src);
    }

    return 0;
}