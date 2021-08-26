#include <stdint.h>

#include <math.h>

#include "../sound_io.h"
#include "../track/track.h"
#include "../../utils/util.h"

#define PI 3.14159265358979323846

void tremolo(TRACK t, uint64_t length, double speed, double range, int32_t sampling_freq) {
    for (uint64_t n = 0; n < length; n++) {
        t->data[n] *= (0.8 + range * sin((2 * PI * speed * n) / sampling_freq));
    }
}