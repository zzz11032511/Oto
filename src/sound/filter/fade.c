#include <stdint.h>

#include "../sound_io.h"
#include "../track/track.h"
#include "../../utils/util.h"

void fade_in(TRACK t, uint64_t length, double time, int32_t sampling_freq) {
    double fade_time = sampling_freq * time;
    for (uint64_t n = 0; n < fade_time; n++) {
        if (n >= length) {
            return;
        }
        t->data[n] *= (double)n / fade_time;
    }
}

void fade_out(TRACK t, uint64_t length, double time, int32_t sampling_freq) {
    double fade_time = sampling_freq * time;
    for (uint64_t n = 0; n < fade_time; n++) {
        if (n >= length) {
            return;
        }
        t->data[length - n - 1] *= (double)n / fade_time;
    }
}