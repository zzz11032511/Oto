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

uint32_t count_string_size(str_t s, uint8_t end_ch);

/**
 * pathに指定されたソースファイルをsrcに読み込む
 * 正常終了したときはファイルサイズを返す 
 */
uint32_t src_load(const str_t path, str_t *src);

/* int64_t型の変数を、bit列はそのままにdoubleに変換する */
double itod(int64_t i);

/* double型の変数を、bit列はそのままにint64_tに変換する */
int64_t dtoi(double d);

int32_t count_bits(int64_t bits);
