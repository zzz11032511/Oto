#pragma once

#include <stdint.h>

typedef uint8_t *str_t;    // 文字列

#define SOURCE_BUF_SIZE 10000    // ファイルから読み取ったソースを確保する領域のサイズ

/**
 * pathに指定されたソースファイルをsrcに読み込む
 *  
 * args:
 *     path : ソースファイルのパス
 *     src  : ファイルを読み込む先の場所
 *     size : srcのサイズ
 */
int32_t srcLoad(str_t path, str_t t, int32_t size);