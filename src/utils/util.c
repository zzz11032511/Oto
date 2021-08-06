#include "util.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* fpが指しているファイルのバイト数をしらべる関数 */
uint32_t count_srcsize(FILE *fp) {
    uint32_t cnt = 0;

    while (1) {
        int8_t ch = fgetc(fp);
        if (ch == EOF) break;
        cnt++;
    }

    rewind(fp);
    return cnt;
}

/* ファイルがotoの拡張子かを判定する */
bool_t is_otofile(const str_t path) {
    const char *ext = strrchr(path, '.');
    int32_t is_oto = strcmp(ext, ".oto");

    if (!is_oto) return true;
    return false;
}

/* pathに指定されたソースファイルをsrcに読み込む */
int32_t src_load(const str_t path, str_t *src) {
    if (!is_otofile(path)) {
        fprintf(stderr, "%s is not Oto file\n", path);
        return 1;
    }

    FILE *fp = fopen(path, "rt");
    if (fp == 0) {
        fprintf(stderr, "fopen error : %s\n", path);
        return 1;
    }

    int32_t size = count_srcsize(fp);

#ifdef DEBUG
    printf("Source file info\n");
    printf("name : %s, size : %d bytes\n\n", path, size);
#endif

    *src = (str_t)malloc(sizeof(char) * (size + 1));
    if (src == NULL) {
        fprintf(stderr, "malloc error\n");
        return 1;
    }
    
    int32_t end = fread(*src, 1, size, fp);  // ファイルの終端のidxが返る

    fclose(fp);
    src[end] = 0;  // 終端マークを置いておく

    return 0;
}

union di_u {
    int64_t i;
    double d;
};

/* int64_t型の変数を、bit列はそのままにdoubleに変換する */
double itod(int64_t i) {
    union di_u u;
    u.i = i;
    return u.d;
}

/* double型の変数を、bit列はそのままにint64_tに変換する */
int64_t dtoi(double d) {
    union di_u u;
    u.d = d;
    return u.i;
}
