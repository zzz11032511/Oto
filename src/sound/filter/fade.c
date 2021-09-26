#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "filter.h"
#include "../track.h"
#include "../../util/util.h"

void fade_in(TRACK t, double time) {
    uint64_t len = t->length;
    double fade_time = t->samples_per_sec * time;
    for (uint64_t n = 0; n < fade_time; n++) {
        if (n >= len) {
            return;
        }
        t->data[n] *= (double)n / fade_time;
    }
}

void fade_out(TRACK t, double time) {
    uint64_t len = t->length;
    double fade_time = t->samples_per_sec * time;
    for (uint64_t n = 0; n < fade_time; n++) {
        if (n >= len) {
            return;
        }
        t->data[len - n - 1] *= (double)n / fade_time;
    }
}

void fade(TRACK t, double start, double end) {
    fade_in(t, start);
    fade_out(t, end);
}