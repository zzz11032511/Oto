#pragma once

#include <stdint.h>

#include "track/track.h"

void play(double freq, double second, uint8_t velocity, 
          int32_t wave, int32_t channel, int32_t sampling_freq);
