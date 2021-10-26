#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <mymacro.h>

#include "filter.h"
#include "sound.h"
#include "../error/error.h"

Filter *new_filter(filter_num_t filter_num, uint64_t param) {
    Filter *f = MYMALLOC1(Filter);
    if (f == NULL) {
        call_error(UNKNOWN_ERROR, "new_filter()");
    }
    f->filter_num = filter_num;
    f->param = param;

    return f;
}

static const struct init_define_filters def_filters[] = {
    {"NONE_FILTER", NONE_FILTER, 0},
    {"CLIP",        CLIP,        0},
    {"FADE",        FADE,        2},
    {"FADE_IN",     FADE_IN,     1},
    {"FADE_OUT",    FADE_OUT,    1},
    {"AMP",         AMP,         1},
    {"TREMOLO",     TREMOLO,     2},
};

void init_filter() {
    size_t num_of_filters = GET_ARRAY_LENGTH(def_filters);
    for (int32_t i = 1; i < num_of_filters; i++) {
        size_t str_len = strlen(def_filters[i].s);
        tokencode_t tc = allocate_tc(def_filters[i].s, str_len, TyFilter);

        Filter *f = new_filter(def_filters[i].filter_num, def_filters[i].param);

        assign_pointer(tc, TyFilter, (void *)f);
    }
}

OTO_FILTER clip(float *d) {
    if (*d > 1) *d = 1;
    else if (*d < -1) *d = -1;
}

OTO_FILTER fade_in(float *d, uint64_t t, uint64_t length, double time) {
    if (t < (time * length)) {
        *d *= t / (time * length);
    }
}

OTO_FILTER fade_out(float *d, uint64_t t, uint64_t length, double time) {
    if ((length - t) < (time * length)) {
        *d *= (length - t) / (time * length);
    }
}

OTO_FILTER fade(float *d, uint64_t t, uint64_t length, double start, double end) {
    fade_in(d, t, length, start);
    fade_out(d, t, length, end);
}

/* 最初に定義しておくフィルタ */
struct init_define_filters {
    int8_t *s;
    uint64_t filter_num;
    uint64_t param;
};
static const struct init_define_filters def_filters[] = {
    {"NONE_FILTER", NONE_FILTER, 0},
    {"CLIP",        CLIP,        0},
    {"FADE",        FADE,        2},
    {"FADE_IN",     FADE_IN,     1},
    {"FADE_OUT",    FADE_OUT,    1},
    {"AMP",         AMP,         1},
    {"TREMOLO",     TREMOLO,     2},
};

/* クリックノイズ除去用のフェードの幅 */
#define FADE_RANGE 0.05

OTO_FILTER io_fade(float *d, uint64_t t, uint64_t length) {
    fade_in(d, t, length, FADE_RANGE);
    fade_out(d, t, length, FADE_RANGE);
}

void filtering(float *d, Sound *s, uint64_t t, uint64_t length,
               uint64_t sampling_freq) {
    uint64_t i = 0;
    uint64_t end = s->num_of_filter;

    while (i < end) {
        if (s->filters[i]->type != TyFilter) {
            call_error(SOUND_PLAYER_ERROR);
        }
        Filter *f = (Filter *)s->filters[i]->value.pVal;

        filter_num_t filter_num = f->filter_num;
        uint64_t param = f->param;

        switch (filter_num) {
        case CLIP:
            clip(d);
            break;
        case FADE_IN:
            fade_in(d, t, length, 
                s->filters[i + 1]->value.fVal
            );
            break;
        case FADE_OUT:
            fade_out(d, t, length, 
                s->filters[i + 1]->value.fVal
            );
            break;
        case FADE:
            fade(d, t, length,
                s->filters[i + 1]->value.fVal,
                s->filters[i + 2]->value.fVal
            );
            break;
        case AMP:
            break;
        case TREMOLO:
            break;
        default:
            call_error(UNKNOWN_ERROR, "filtering()");
        }

        i += param + 1;
    }
}