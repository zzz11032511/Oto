#include <stdio.h>
#include <stdlib.h>

#include "track.h"
#include "oscillator.h"
#include "../util/util.h"
#include "../error/error.h"

Oscillator *new_oscillator(basicwave_t wave, Oscillator *fm, Oscillator *am) {
    Oscillator *osc = MYMALLOC1(Oscillator);
    if (osc == NULL) {
        call_error(UNKNOWN_ERROR, "new_oscillator()");
    }

    osc->wave = wave;
    osc->fm   = fm;
    osc->am   = am;

    return osc;
}

void free_oscillator(Oscillator *osc) {
    free(osc);
}