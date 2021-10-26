#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "sound.h"
#include "oscillator.h"
#include "../util/util.h"
#include "../error/error.h"

Sound *new_sound(Oscillator *oscillator) {
    Sound *s = MYMALLOC1(Sound);
    if (s == NULL) {
        call_error(UNKNOWN_ERROR, "new_sound()");
    }

    s->oscillator = oscillator;
    s->num_of_filter = 0;

    return s;
}

void free_sound(Sound *s) {
    free(s);
}