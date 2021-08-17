#include "filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

FILTER new_filter(int32_t filter_num) {
    FILTER s = calloc(1, sizeof(struct filter));
    s->filter_num = filter_num;
    s->param      = 0;
    s->next_ftr   = NULL;
    return s;
}
