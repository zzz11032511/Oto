#pragma once

#include <stdint.h>

#include "../sound.h"
#include "../track/track.h"
#include "../../utils/util.h"
#include "../../lexer/token.h"
#include "../../variable/variable.h"

enum FILTER_NUMBER {
    NONE_FILTER = 0,
    FADE_IN,
    FADE_OUT,
    AMP,
    TREMOLO,
    ADSR,
    LOW_PASS,
    HIGH_PASS,
    VIBRATO,
    WAH,
    DELAY,
    REVERB,
};

struct filter {
    int32_t filter_num;
    int32_t type;
    int32_t param;  // フィルタのパラメータの数
};
typedef struct filter *FILTER;

FILTER new_filter(int32_t filter_num, int32_t param);
void init_filter(tokenbuf_t *tcbuf, var_t *var_list);

/* fade.c */
void fade_in(TRACK t, uint64_t length, double time, int32_t sampling_freq);
void fade_out(TRACK t, uint64_t length, double time, int32_t sampling_freq);

/* tremolo.c */
void tremolo(TRACK t, uint64_t length, double speed, double range, int32_t sampling_freq);

/* amp.c */
void amp(TRACK t, uint64_t length, double times, int32_t sampling_freq);
