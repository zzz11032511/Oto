#pragma once

#include <stdint.h>

#include "../sound.h"
#include "../track/track.h"
#include "../../utils/util.h"

enum FILTER_NUMBER {
    FADE_IN,
    FADE_OUT,
    AMP,
    TREMOLO
};

struct filter {
    int32_t filter_num;
    int32_t param;  // フィルタのパラメータの数
    struct filter *next_ftr;
};
typedef struct filter *FILTER;

FILTER new_filter(int32_t filter_num, int32_t param);

/* fade.c */
void fade_in(TRACK t, uint64_t length, double time, int32_t sampling_freq);
void fade_out(TRACK t, uint64_t length, double time, int32_t sampling_freq);

/* tremolo.c */
void tremolo(TRACK t, uint64_t length, double speed, int32_t sampling_freq);

/* amp.c */
void amp(TRACK t, uint64_t length, double times, int32_t sampling_freq);
