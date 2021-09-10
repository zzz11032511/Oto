#pragma once

#include <stdint.h>

/**
 * 先頭が@である文字列を渡すと, プリプロセスを実行する
 * 
 * プリプロセッサ命令全体の文字数(改行含む)を返すので
 * lexer側では i += preprocess() と呼び出す
 */
uint32_t preprocess(int8_t *pre_start);