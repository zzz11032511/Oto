#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

size_t count_string_length(char *s, char endch) {
    size_t size = 0;
    while (s[size] != endch) {
        if (s[size] == '\0') return size;
        size++;
    }
    return size;
}

size_t count_file_size(const char *filename) {
    FILE *fp = fopen(filename, "rt");
    if (fp == 0) {
        return 0;
    }

    size_t size = 0;
    while (1) {
        int32_t ch = fgetc(fp);
        if (ch == EOF) break;
        size++;
    }

    fclose(fp);

    return size;
}

void line_print(char *s) {
    int32_t i = 0;
    while (s[i] != '\n' && s[i] != EOF && s[i] != 0) {
        printf("%c", s[i]);
        i++;
    }
    printf("\n");
}

bool is_otofile(const char *filename) {
    const char *ext = strrchr(filename, '.');

    if (strcmp(ext, ".oto") == 0) return true;
    return false;
}

char *src_open(const char *filename) {
    size_t fsize = count_file_size(filename);

    FILE *fp = fopen(filename, "rt");
    if (fp == 0) {
        return NULL;
    }

    // srcの終端に'\0'を入れるために(fsize + 1)
    char *src = (char *)malloc(sizeof(char) * (fsize + 1));
    if (src == NULL) {
        return NULL;
    }

    int32_t end = fread(src, 1, fsize, fp);
    src[end] = 0;

    fclose(fp);

#ifdef DEBUG
    printf("Source file info\n");
    printf("name : %s, size : %I64d bytes\n\n", filename, fsize);
#endif

    return src;
}