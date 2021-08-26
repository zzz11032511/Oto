#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

SOUND new_sound(int32_t wave) {
    SOUND s = calloc(1, sizeof(struct sound));
    s->wave = wave;
    s->filter_ptr = 0;

    return s;
}
