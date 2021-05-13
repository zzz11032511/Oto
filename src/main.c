#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h"

int main(int argc, const char **argv)
{
    if (argc <= 1) {
        printf("no input files");
        exit(1);
    }

    FILE *fp;
    int i;
    const int SIZE = 10000;     // srcfile用のメモリ領域の大きさ  
    unsigned char txt[SIZE];    // srcfile用のメモリ領域
    
    int tc[SIZE];    // トークンコード列

    fp = eOpen(argv[1], "rt");
    
    fclose(fp);

    return 0;
}