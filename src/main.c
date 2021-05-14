#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h"
#include "lexer.h"

int main(int argc, const char **argv)
{
    if (argc <= 1) {
        printf("no input files");
        exit(1);
    }

    FILE *fp;
    int i;
    const int BUF_SIZE = 10000;     // srcfile用のメモリ領域の大きさ  
    unsigned char txt[BUF_SIZE];    // srcfile用のメモリ領域
    
    const int TC_LIST_SIZE = 10000; // トークンコード列に変換したプログラムの長さ
    int tc[TC_LIST_SIZE];           // トークンコード列に変換したプログラム

    lexer(tcInit, tc);    // あらかじめtcInitをlexer()する

    
    fp = eOpen(argv[1], "rt");
    
    

    fclose(fp);

    return 0;
}