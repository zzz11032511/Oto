#include "track.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

TRACK new_track(uint64_t length) {
    TRACK s = (TRACK)calloc(1, sizeof(struct track_t));

    s->length = length;
    s->ptr    = 0;
    s->data   = (double *)calloc(length, sizeof(double));
    
    return s;
}

void free_track(TRACK s) {
    free(s->data);
    s->data = NULL;
    free(s);
}