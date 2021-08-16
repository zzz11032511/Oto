#pragma once

#include <stdint.h>
#include "../track/track.h"
#include "../../utils/util.h"

/* 基本波波形 */
enum FUNDAMENTAL_WAVE {
    SINE_WAVE,
    SAWTOOTH_WAVE,
    SQUARE_WAVE,
    TRIANGLE_WAVE,
    PSG_SAW_WAVE,
    PSG_SQUARE_WAVE,
    PSG_TRIANGLE_WAVE,
    WHITE_NOISE,
};

void write_wave(int32_t wave, 
                TRACK t, double freq, uint64_t length,
                int32_t sampling_freq, bool_t fade_flag, bool_t seek_cursol_flag);
                