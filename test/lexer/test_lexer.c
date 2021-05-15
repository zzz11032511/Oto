#include <stdio.h>
#include <string.h>
#include "../../src/util.h"
#include "../../src/lexer.h"

int main(void)
{
    int tc[100];
    int var[100];

    char t[] = "abs+ - ; ++ fdsa / ()(({{}}";

    lexer(t, tc, var);

    for (int i = 0; i < 100; i++) {
        printf("tc = %u, token = %s\n", tc[i], t[i]);
    }

    return 0;
}