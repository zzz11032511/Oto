#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

SOUND new_sound(int32_t wave) {
    SOUND s = calloc(1, sizeof(struct sound));
    s->wave = wave;
    s->prev = NULL;
    s->next = NULL;
    s->num_of_filter = 0;

    return s;
}