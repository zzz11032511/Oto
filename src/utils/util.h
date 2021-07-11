#pragma once

#include <stdint.h>

/* 文字列 */
typedef char *str_t;

/* ファイルから読み取ったソースを確保する領域のサイズ */
#define SOURCE_BUF_SIZE 10000

/**
 * pathに指定されたソースファイルをsrcに読み込む
 *
 * args:
 *     path : ソースファイルのパス
 *     src  : ファイルを読み込む先の場所
 *     size : srcのサイズ
 */
int32_t srcLoad(const str_t path, str_t t, int32_t size);

/**
 * ファイルがotoの拡張子かを判定する
 * otoのファイルであれば1を返す
 *  
 * args : 
 *     path : ソースファイルのパス
 */
int32_t isOtoFile(const str_t path);