#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "filter.h"
#include "../track.h"
#include "../../util/util.h"

#define CUTOFF_GAIN 0.70710678118654752440

void iir_lpf(TRACK t, double fc) {
    double a[3] = {0};
    double b[3] = {0};

    fc = tan(PI * fc) / (2.0 * PI);
    a[0] = 1.0 + 2.0 * PI * fc / CUTOFF_GAIN + 4.0 * PI * PI * fc * fc;
    a[1] = (8.0 * PI * PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * PI * fc / CUTOFF_GAIN + 4.0 * PI * PI * fc * fc) / a[0];
    b[0] = 4.0 * PI * PI * fc * fc / a[0];
    b[1] = 8.0 * PI * PI * fc * fc / a[0];
    b[2] = 4.0 * PI * PI * fc * fc / a[0];
    a[0] = 1.0;

    double t0[t->length];

    for (uint64_t n = 0; n < t->length; n++) {
        for (uint64_t m = 0; m <= 2; m++) {
            if (n - m >= 0) {
                t0[n] = t->data[n] + b[m] * t->data[n - m];
            }
        }
        for (uint64_t m = 1; m <= 2; m++) {
            if (n - m >= 0) {
                t0[n] = t->data[n] - a[m] * t0[n - m];
            }
        }
        t->data[n] = t0[n];
    }
}

void iir_hpf(TRACK t, double fc) {
    double a[3] = {0};
    double b[3] = {0};

    fc = tan(PI * fc) / (2.0 * PI);
    a[0] = 1.0 + 2.0 * PI * fc / CUTOFF_GAIN + 4.0 * PI * PI * fc * fc;
    a[1] = (8.0 * PI * PI * fc * fc - 2.0) / a[0];
    a[2] = (1.0 - 2.0 * PI * fc / CUTOFF_GAIN + 4.0 * PI * PI * fc * fc) / a[0];
    b[0] = 1.0 / a[0];
    b[1] = -2.0 / a[0];
    b[2] = 1.0 / a[0];
    a[0] = 1.0;

    double t0[t->length];

    for (uint64_t n = 0; n < t->length; n++) {
        for (uint64_t m = 0; m <= 2; m++) {
            if (n - m >= 0) {
                t0[n] = t->data[n] + b[m] * t->data[n - m];
            }
        }
        for (uint64_t m = 1; m <= 2; m++) {
            if (n - m >= 0) {
                t0[n] = t->data[n] - a[m] * t0[n - m];
            }
        }
        t->data[n] = t0[n];
    }
}