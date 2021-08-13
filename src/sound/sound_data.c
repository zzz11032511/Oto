#include "sound_data.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

SOUND new_sound(uint64_t length) {
    SOUND s = (SOUND)calloc(1, sizeof(SOUND_OBJECT));

    s->length = length;
    s->ptr    = 0;
    s->data   = (double *)calloc(length, sizeof(double));
    
    return s;
}

void free_sound(SOUND s) {
    free(s->data);
    s->data = NULL;
    free(s);
}

void rewind_sound(SOUND s) {
    s->ptr = 0;
}

void seek_sound(SOUND s, uint64_t offset) {
    s->ptr += offset;
}