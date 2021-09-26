#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "filter.h"
#include "../track.h"
#include "../../util/util.h"

#define PI 3.14159265358979323846

void amp(TRACK t, double gain) {
    for (uint64_t n = 0; n < t->length; n++) {
        t->data[n] *= gain;
        if (t->data[n] > 1.0) {
            t->data[n] = 1.0;
        } else if (t->data[n] < -1.0) {
            t->data[n] = -1.0;
        }
    }
}

void tremolo(TRACK t, double speed, double range) {
    for (uint64_t n = 0; n < t->length; n++) {
        t->data[n] *= (0.8 + range * sin((2 * PI * speed * n) / t->samples_per_sec));
    }
}