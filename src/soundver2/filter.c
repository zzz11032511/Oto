#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "filter.h"

OTO_FILTER clip(float *d) {
    if (*d > 1) *d = 1;
    else if (*d < -1) *d = -1;
}

OTO_FILTER fade_in(float *d, uint64_t t, uint64_t length, double time) {
    if (t < (time * length)) {
        *d *= t / (time * length);
    }
}

OTO_FILTER fade_out(float *d, uint64_t t, uint64_t length, double time) {
    if ((length - t) < (time * length)) {
        *d *= (length - t) / (time * length);
    }
}

OTO_FILTER fade(float *d, uint64_t t, uint64_t length, double start, double end) {
    fade_in(d, t, length, start);
    fade_out(d, t, length, end);
}