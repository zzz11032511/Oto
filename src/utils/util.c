#include "util.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* fileのバイト数をしらべる関数 */
int32_t countSrcByte(const char *path) {
    FILE *fp = fopen(path, "rt");
    if (fp == 0) {
        fprintf(stderr, "fopen error : %s\n", path);
        exit(1);
    }

    int32_t cnt = 0;

    while (1) {
        int8_t ch = fgetc(fp);
        if (ch == EOF) break;
        cnt++;
    }

    fclose(fp);

    return cnt;
}

/* pathに指定されたソースファイルをsrcに読み込む */
int32_t srcLoad(const str_t path, str_t src, int32_t size) {

#ifdef DEBUG
    printf("Source file info\n");
    printf("name : %s, size : %d bytes\n\n", path, countSrcByte(path));
#endif
    if (!isOtoFile(path)) {
        fprintf(stderr, "%s is not Oto file\n", path);
        return 1;
    }

    FILE *fp = fopen(path, "rt");
    if (fp == 0) {
        fprintf(stderr, "fopen error : %s\n", path);
        return 1;
    }
    int32_t end = fread(src, 1, size - 1, fp);  // ファイルの終端のidxが返る
    fclose(fp);

    src[end] = 0;  // 終端マークを置いておく

    return 0;
}

/* ファイルがotoの拡張子かを判定する */
int32_t isOtoFile(const str_t path) {
    const char *ext = strrchr(path, '.');

    int32_t isOto = strcmp(ext, ".oto");
    if (!isOto) {
        return 1;
    }

    return 0;
}