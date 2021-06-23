#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "util.h"

/**
 * pathに指定されたソースファイルをsrcに読み込む
 *  
 * args:
 *     path : ソースファイルのパス
 *     src  : ファイルを読み込む先の場所
 *     size : srcのサイズ
 */
int32_t srcLoad(str_t path, str_t src, int size)
{
    FILE *fp = fopen(path, "rt");
    if (fp == 0) {
        fprintf(stderr, "fopen error : %s\n", path);
        return 1;
    }
    int32_t end = fread(src, 1, size - 1, fp);    // ファイルの終端のidxが返る
    fclose(fp);

    src[end] = 0;    // 終端マークを置いておく

    return 0;
}
