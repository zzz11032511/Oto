#include "variable.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void init_var_list(var_t *var_list, uint32_t size) {
    uint32_t i = 0;
    while (i < size) {
        var_list[i].type       = TyVoid;
        var_list[i].value.pVal = NULL;
        var_list[i].tc         = 0;
        i++;
    }
}

void free_heap_var(var_t *var_list, uint32_t size) {
    uint32_t i = 0;
    while (i < size) {
        if (var_list[i].type == TySound || var_list[i].type == TyFilter) {
            free(var_list[i].value.pVal);
            var_list[i].value.pVal = NULL;
        }
        i++;
    }
}
