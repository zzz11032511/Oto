#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "filter.h"
#include "../util/util.h"
#include "../error/error.h"

filter *new_filter(filter_num_t filter_num, uint64_t param) {
    filter *f = MYMALLOC(filter);
    if (f == NULL) {
        call_error(UNKNOWN_ERROR, "new_filter()");
    }

    f->filter_num = filter_num;
    f->param = param;

    return f;
}

void free_filter(filter *f) {
    free(f);
}