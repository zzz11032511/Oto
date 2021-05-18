#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "../src/util.h"
#include "../src/lexer.h"

int main(void)
{
    int tc[100];
    int var[100];

    char t[] = {'a', ' ', '=', ' ', '0', 0};

    int l = lexer(t, tc, var);
    assert(l == 3);

    for (int i = 0; i < l; i++) {
        printf("tc : %d\n", tc[i]);
    }
    printf("%d\n", l);
    
    return 0;
}