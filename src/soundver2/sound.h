#pragma once

#include <stdint.h>

#include "oscillator.h"
#include "../variable/variable.h"

#define MAX_CONNECT 250

/* 音色情報 */
typedef struct {
    Oscillator *oscillator;

    var_t *filters[MAX_CONNECT];
    size_t num_of_filter;
} Sound;

Sound *new_sound(Oscillator *oscillator);
void free_sound(Sound *s);