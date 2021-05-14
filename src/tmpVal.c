#include <stdio.h>
#include "tmpVal.h"
#include "lexer.h"

char tmp_flag[TMP_FLAG_LIST_SIZE];    // 一時変数の利用状況

/**
 *  未使用の一時変数の確保
 *  対応する一時変数のトークンコードを返す
 */
int tmpAlloc()
{
    int i;
    for (i = 0; i < 10; i++) {
        if (tmp_flag[i] == 0) break;
    }
    if (i >= 10) {
        printf("tmpAlloc: error\n");
        return -1;
    }
    tmp_flag[i] = 1;
    return i + TcTmp0;
}

/**
 * iが一時変数を指すトークンコードであれば解放する
 */
void tmpFree(int i)
{
    if (TcTmp0 <= i && i <= TcTmp9) {
        tmp_flag[i - TcTmp0] = 0;
    }
}