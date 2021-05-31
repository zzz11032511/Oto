#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "lexer.h"
#include "variable.h"

#define MAX_TMP_VAR 10

char tmp_flag[MAX_TMP_VAR];    // 一時変数の使用状況

/* 未使用の一時変数の確保 */
int tmpAlloc()
{
    int i;
    for (i = 0; i < MAX_TMP_VAR; i++) {
        if (tmp_flag[i] == 0) break;
    }

    if (i >= MAX_TMP_VAR) {
        printf("tmpAlloc error\n");
        return -1;
    }

    tmp_flag[i] = 1;
    return i + TcTmp0;
}

/* iが一時変数を指すトークンコードであれば解放する */
void tmpFree(int i)
{
    if (TcTmp0 <= i && i <= TcTmp9) {
        tmp_flag[i - TcTmp0] = 0;
    }
}