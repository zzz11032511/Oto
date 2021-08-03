#include "util.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* fpが指しているファイルのバイト数をしらべる関数 */
int32_t countSrcByte(FILE *fp) {
    int32_t cnt = 0;

    while (1) {
        int8_t ch = fgetc(fp);
        if (ch == EOF) break;
        cnt++;
    }

    rewind(fp);

    return cnt;
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

/* pathに指定されたソースファイルをsrcに読み込む */
int32_t srcLoad(const str_t path, str_t *src) {
    if (!isOtoFile(path)) {
        fprintf(stderr, "%s is not Oto file\n", path);
        return 1;
    }

    FILE *fp = fopen(path, "rt");
    if (fp == 0) {
        fprintf(stderr, "fopen error : %s\n", path);
        return 1;
    }

    int32_t srcSize = countSrcByte(fp);

#ifdef DEBUG
    printf("Source file info\n");
    printf("name : %s, size : %d bytes\n\n", path, srcSize);
#endif

    *src = (str_t)malloc(sizeof(char) * (srcSize + 1));
    if (src == NULL) {
        fprintf(stderr, "malloc error\n");
        exit(1);
    }
    
    int32_t end = fread(*src, 1, srcSize, fp);  // ファイルの終端のidxが返る

    fclose(fp);

    src[end] = 0;  // 終端マークを置いておく

    return 0;
}

union di_u {
    int64_t i;
    double d;
};

/* int64_t型の変数を、bit列はそのままにdoubleに変換する */
double itod(int64_t i){
    union di_u u;
    u.i = i;
    return u.d;
}

/* double型の変数を、bit列はそのままにint64_tに変換する */
int64_t dtoi(double d){
    union di_u u;
    u.d = d;
    return u.i;
}


int32_t toLower(char c) {
    if ('A' <= c && c <= 'Z') return (c + 0x20);
    return c;
}

int32_t strncmp_ignorecase(const str_t s1, const str_t s2, int32_t n) {
    int32_t i = 0;
    while (toLower(s1[i]) == toLower(s2[i])) {
        if (s1[i] == '\0' || i >= n) return 0;
        i++;
    }

    return toLower(s1[i]) - toLower(s2[i]);
}