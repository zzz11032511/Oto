#include "track.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

TRACK new_track(uint64_t length, uint64_t channel, uint64_t samples_per_sec,
                uint64_t bits_per_sample, uint64_t velocity) {
    TRACK s = (TRACK)calloc(1, sizeof(struct track_t));

    s->length   = length;
    s->ptr      = 0;
    s->data     = (double *)calloc(length, sizeof(double));
    s->channel  = channel;
    s->velocity = velocity;
    s->samples_per_sec = samples_per_sec;
    s->bits_per_sample = bits_per_sample;
    
    return s;
}

void free_track(TRACK s) {
    free(s->data);
    s->data = NULL;
    free(s);
}