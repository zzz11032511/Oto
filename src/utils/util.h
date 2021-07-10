#pragma once

#include <stdint.h>

typedef char *str_t;  // 文字列

// ファイルから読み取ったソースを確保する領域のサイズ
#define SOURCE_BUF_SIZE 10000

/**
 * pathに指定されたソースファイルをsrcに読み込む
 *
 * args:
 *     path : ソースファイルのパス
 *     src  : ファイルを読み込む先の場所
 *     size : srcのサイズ
 */
int32_t srcLoad(const char *path, str_t t, int32_t size);