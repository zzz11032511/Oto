#pragma once

#include <stdint.h>

#include "../sound_io.h"
#include "../track/track.h"
#include "../../utils/util.h"

void fade_in(TRACK t, uint64_t length, double time, int32_t sampling_freq);
void fade_out(TRACK t, uint64_t length, double time, int32_t sampling_freq);
