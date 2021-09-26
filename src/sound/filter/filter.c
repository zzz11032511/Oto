#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "filter.h"
#include "../sound.h"
#include "../track.h"
#include "../../util/util.h"
#include "../../error/error.h"
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
    {"FADE",        FADE,        2},
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

void filtering(TRACK t, SOUND s) {
    int32_t i = 0;
    int32_t end = s->num_of_filter;

    while (i < end) {
        if (s->filters[i].type != TyFilter) {
            call_error(SOUND_PLAYER_ERROR);
        }
        FILTER f = (FILTER)s->filters[i].value.pVal;

        filter_num_t filter_num = f->filter_num;
        int32_t param = f->param;

        DEBUG_IPRINT(filter_num);

        switch (filter_num) {
        case FADE:
            fade(t,
                s->filters[i + 1].value.fVal,
                s->filters[i + 2].value.fVal
            );
            break;
        case FADE_IN:
            fade_in(t, 
                s->filters[i + 1].value.fVal
            );
            break;
        case FADE_OUT:
            fade_out(t, 
                s->filters[i + 1].value.fVal
            );
            break;
        case AMP:
            amp(t, 
                s->filters[i + 1].value.fVal
            );
            break;
        case TREMOLO:
            tremolo(t, 
                s->filters[i + 1].value.fVal,
                s->filters[i + 2].value.fVal
            );
            break;
        case ADSR:
            adsr(t,
                s->filters[i + 1].value.fVal,
                s->filters[i + 2].value.fVal,
                s->filters[i + 3].value.fVal,
                s->filters[i + 4].value.fVal
            );
            break;
        case LOW_PASS:
        case HIGH_PASS:
        case VIBRATO:
        case WAH:
        case DELAY:
        case REVERB:     
        default:
            call_error(SOUND_PLAYER_ERROR);
        }
        
        i += param + 1;
    }
}