#include "filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

FILTER new_filter(int32_t filter_num, int32_t param) {
    FILTER s = calloc(1, sizeof(struct filter));
    s->filter_num = filter_num;
    s->param      = param;
    s->next_ftr   = NULL;
    return s;
}
