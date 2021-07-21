#pragma once

#include <stdint.h>

/* 文字列 */
typedef char *str_t;

/* 真理値型 */
typedef int boolean;
#define true     1
#define false    0

#define GET_ARRAY_LENGTH(a) (sizeof(a)/sizeof(a[0]))

/**
 * pathに指定されたソースファイルをsrcに読み込む
 *
 * args:
 *     path : ソースファイルのパス
 *     src  : ファイルを読み込む先の場所
 */
int32_t srcLoad(const str_t path, str_t *src);