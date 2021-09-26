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

void adsr(TRACK t, double attack, double decay, double sustain, double release) {
    uint64_t n = 0;

    uint64_t A = attack * t->samples_per_sec;
    uint64_t D = decay * t->samples_per_sec;
    uint64_t R = release * t->samples_per_sec;

    uint64_t gate = t->length - R;

    if (A != 0) {
        while (n < A) {
            t->data[n] *= 1.0 - exp(-5.0 * n / A);
            n++;
        }
    }

    if (D != 0) {
        while (n < gate && n < t->length) {
            t->data[n] *= sustain + (1 - sustain) * exp(-5.0 * (n - A) / D);
            n++;
        }

    } else {
        while (n < gate && n < t->length) {
            t->data[n] *= sustain;
            n++;
        }
    }

    if (R != 0) {
        while (n < t->length) {
            t->data[n] *= exp(-5.0 * (n - gate + 1) / R);
            n++;
        }
    }
}