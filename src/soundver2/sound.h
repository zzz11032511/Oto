#pragma once

#include <stdint.h>

#include "oscillator.h"
#include "../variable/variable.h"

#define MAX_CONNECT 250

struct sound {
    oscillator *oscillator;

    var_t *filters[MAX_CONNECT];
    int32_t num_of_filter;
};
typedef struct sound sound;

sound *new_sound(oscillator *oscillator);
void free_sound(sound *s);