#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* エラーチェック付きのfopen */
FILE *eOpen(char *filename, char *mode)
{
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        perror(filename);
        exit(1);
    }
    return fp;
}