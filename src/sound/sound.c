#include "sound.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

SOUND init_sound(uint64_t length) {
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
    return;
}

void rewind_sound(SOUND s) {
    s->ptr = 0;
    return;
}
