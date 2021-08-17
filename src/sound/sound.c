#include "sound.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

SOUND new_sound(int32_t wave) {
    SOUND s = calloc(1, sizeof(struct sound));
    s->wave = wave;
    s->next_ftr = NULL;
    return s;
}
