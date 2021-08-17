#pragma once

#include <stdint.h>

#include "filter/filter.h"

struct sound {
    int32_t wave;
    struct filter *next_ftr;
};
typedef struct sound *SOUND;

SOUND new_sound(int32_t wave);