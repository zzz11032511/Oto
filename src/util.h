#ifndef INCLUDED_UTIL
#define INCLUDED_UTIL 1

typedef unsigned char *String;

#define SOURCE_BUF_SIZE 10000    // ファイルから読み取ったソースを確保する領域のサイズ

/**
 * pathに指定されたソースファイルをsrcに読み込む
 *  
 * args:
 *     path : ソースファイルのパス
 *     src  : ファイルを読み込む先の場所
 *     size : srcのサイズ
 */
int srcLoad(String path, String t, int size);

#endif