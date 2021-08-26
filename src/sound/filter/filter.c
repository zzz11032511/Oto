#include "filter.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../../utils/util.h"
#include "../../lexer/token.h"
#include "../../variable/variable.h"

/**
 *  最初に定義しておくフィルタ
 */
struct init_define_filters {
    str_t s;
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


FILTER new_filter(int32_t filter_num, int32_t param) {
    FILTER s = calloc(1, sizeof(struct filter));
    s->filter_num = filter_num;
    s->param      = param;
    return s;
}

void init_filter(tokenbuf_t *tcbuf, var_t *var_list) {
    int32_t num_of_filters = GET_ARRAY_LENGTH(def_filters);
    for (int32_t i = 1; i < num_of_filters; i++) {
        define_var(tcbuf, var_list,
                   def_filters[i].s, TyFilter, def_filters[i].filter_num);
    }
}
