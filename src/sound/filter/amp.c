#include <stdint.h>

#include "../sound_io.h"
#include "../track/track.h"
#include "../../utils/util.h"

void amp(TRACK t, uint64_t length, double times, int32_t sampling_freq) {
    for (uint64_t n = 0; n < length; n++) {
        t->data[n] *= times;
        if (t->data[n] > 1.0) {
            t->data[n] = 1.0;
        } else if (t->data[n] < -1.0) {
            t->data[n] = -1.0;
        }
    }
}