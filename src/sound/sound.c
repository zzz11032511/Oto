#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

SOUND new_sound(int32_t wave) {
    SOUND s = calloc(1, sizeof(struct sound_t));
    s->wave = wave;
    return s;
}
