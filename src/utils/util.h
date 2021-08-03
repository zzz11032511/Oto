#pragma once

#include <stdint.h>

#ifdef TIME
#include <time.h>
#endif

/* 文字列 */
typedef char *str_t;

/* 真理値型 */
typedef int boolean;
#define true     1
#define false    0

#define GET_ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))


#ifdef TIME
#define CALC_TIME(s, e) ((double)(e - s) / CLOCKS_PER_SEC)
#endif

/**
 * pathに指定されたソースファイルをsrcに読み込む
 *
 * args:
 *     path : ソースファイルのパス
 *     src  : ファイルを読み込む先の場所
 */
int32_t srcLoad(const str_t path, str_t *src);

/* int64_t型の変数を、bit列はそのままにdoubleに変換する */
double itod(int64_t i);

/* double型の変数を、bit列はそのままにint64_tに変換する */
int64_t dtoi(double d);

/* 大文字小文字の区別をせずに文字列を比較する */
int32_t strncmp_ignorecase(const str_t s1, const str_t s2, int32_t n);