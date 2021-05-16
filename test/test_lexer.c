#include <stdio.h>
#include <string.h>
#include "../src/util.h"
#include "../src/lexer.h"

int main(void)
{
    int tc[100];
    int var[100];

    char t[30] = "abs+ - ; ++ fdsa / ()(({{}} ";
    t[29] = 0;

    printf("%d\n", lexer(t, tc, var));

    return 0;
}