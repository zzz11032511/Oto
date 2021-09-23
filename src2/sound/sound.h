#pragma once

#include <stdint.h>

#include "../variable/variable.h"

#define MAX_CONNECT 250

struct sound {
    int32_t wave;
    var_t *filters[MAX_CONNECT];
    int32_t num_of_filter;

    struct sound *prev;
    struct sound *next;
};
typedef struct sound *SOUND;

SOUND new_sound(int32_t wave);