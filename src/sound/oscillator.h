#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "track.h"

typedef enum {
    SINE_WAVE = 0,
    SAWTOOTH_WAVE,
    SQUARE_WAVE,
    TRIANGLE_WAVE,
    PSG_SAW_WAVE,
    PSG_SQUARE_WAVE,
    PSG_TRIANGLE_WAVE,
    WHITE_NOISE,
} wave_t;

void write_wave(int32_t wave, 
                TRACK t, double freq, uint64_t length,
                int32_t sampling_freq, bool fade_flag, bool seek_cursol_flag);