#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "filter.h"
#include "../../util/util.h"
#include "../../token/token.h"
#include "../../variable/type.h"
#include "../../variable/variable.h"

FILTER new_filter(filter_num_t filter_num, uint64_t param) {
    FILTER f = calloc(1, sizeof(struct filter));
    f->filter_num = filter_num;
    f->param = param;

    return f;
}

/* 最初に定義しておくフィルタ */
struct init_define_filters {
    int8_t *s;
    int32_t filter_num;
    int32_t param;
};

static const struct init_define_filters def_filters[] = {
    {"NONE_FILTER", NONE_FILTER, 0},
    {"FADE_IN",     FADE_IN,     1},
    {"FADE_OUT",    FADE_OUT,    1},
    {"AMP",         AMP,         1},
    {"TREMOLO",     TREMOLO,     2},
    {"ADSR",        ADSR,        4},
    {"LOW_PASS",    LOW_PASS,    1},
    {"HIGH_PASS",   HIGH_PASS,   1},
    {"VIBRATO",     VIBRATO,     2},
    {"WAH",         WAH,         2},
    {"DELAY",       DELAY,       2},
    {"REVERB",      REVERB,      2},
};

void init_filter() {
    size_t num_of_filters = GET_ARRAY_LENGTH(def_filters);
    for (int32_t i = 1; i < num_of_filters; i++) {
        size_t str_len = strlen(def_filters[i].s);
        tokencode_t tc = allocate_tc(def_filters[i].s, str_len, TyFilter);

        FILTER f = new_filter(def_filters[i].filter_num, def_filters[i].param);

        assign_pointer(tc, TyFilter, (void *)f);
    }
}