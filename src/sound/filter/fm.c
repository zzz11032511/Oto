#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "filter.h"
#include "../track.h"
#include "../../util/util.h"

void vibrato(TRACK t, double speed, double depth) {
    double dt = t->samples_per_sec * (depth + 0.01);
    depth = depth * t->samples_per_sec;

    double tau = 0;
    double m = 0;
    double delta = 0;

    // 上手くできない
    for (uint64_t n = 0; n < t->length; n++) {
        tau = dt + depth * sin(2.0 * PI * speed * n / t->samples_per_sec);
        m = floor(n + tau);
        delta = n + tau - m;

        if (m > 0 && (m + 1) <= t->length) {
            t->data[n] = delta * t->data[n + 1] + (1.0 - delta) * t->data[n];
            if (t->data[n] > 1) {
                t->data[n] = 1;
            }
        }
    }
}