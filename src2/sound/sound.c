#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "sound.h"
#include "oscillator.h"

SOUND new_sound(wave_t wave, SOUND in_sound, double fm) {
    SOUND s = calloc(1, sizeof(struct sound));
    s->wave = wave;
    s->fm   = fm;
    s->prev = in_sound;
    s->next = NULL;
    s->num_of_filter = 0;

    return s;
}