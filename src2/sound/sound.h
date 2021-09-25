#pragma once

#include <stdint.h>

#include "oscillator.h"
#include "../variable/variable.h"

#define MAX_CONNECT 250

struct sound {
    int32_t wave;
    var_t *filters[MAX_CONNECT];
    int32_t num_of_filter;

    struct sound *prev;
    struct sound *next;
    double fm;  // 変調周波数
};
typedef struct sound *SOUND;

SOUND new_sound(wave_t wave, SOUND in_sound, double fm);