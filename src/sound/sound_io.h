#pragma once

#include <stdint.h>

#include "sound.h"
#include "track/track.h"

void play(double freq, double second, uint8_t velocity, 
          SOUND s, int32_t channel, int32_t sampling_freq);
