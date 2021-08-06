#pragma once

#include <stdint.h>
#include <time.h>

/* 文字列型 */
typedef char *str_t;

/* 真理値型 */
typedef int bool_t;
#define true     1
#define false    0

#define GET_ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

#define CALC_TIME(s, e) ((double)(e - s) / CLOCKS_PER_SEC)

/* pathに指定されたソースファイルをsrcに読み込む */
int32_t src_load(const str_t path, str_t *src);

/* int64_t型の変数を、bit列はそのままにdoubleに変換する */
double itod(int64_t i);

/* double型の変数を、bit列はそのままにint64_tに変換する */
int64_t dtoi(double d);
