#pragma once

#include <stdint.h>

#include "filter/filter.h"
#include "../variable/var_stack.h"

#define MAX_CONNECT 250

struct sound {
    int32_t wave;
    var_t filters[MAX_CONNECT];
    int32_t filter_ptr;
};
typedef struct sound *SOUND;

SOUND new_sound(int32_t wave);