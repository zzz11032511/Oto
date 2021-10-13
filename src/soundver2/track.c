#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "track.h"
#include "../util/util.h"
#include "../error/error.h"

track *new_track(uint64_t length, uint64_t channel, uint64_t samples_per_sec,
                 uint64_t bits_per_sample, uint64_t velocity) {
    track *s = MYMALLOC(track);
    if (s == NULL) {
        call_error(UNKNOWN_ERROR, "new_track()");
    }

    s->length    = length;
    s->wave_data = (double *)calloc(length, sizeof(double));
    s->t         = 0;
    s->channel   = channel;
    s->velocity  = velocity;
    s->samples_per_sec = samples_per_sec;
    s->bits_per_sample = bits_per_sample;
    
    return s;
}

void free_track(track *t) {
    free(t->wave_data);
    free(t);
}