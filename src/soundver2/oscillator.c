#include <stdio.h>
#include <stdlib.h>

#include "oscillator.h"
#include "../util/util.h"
#include "../error/error.h"

oscillator *new_oscillator(basicwave_t wave, oscillator *fm, oscillator *am) {
    oscillator *osc = MYMALLOC(oscillator);
    if (osc == NULL) {
        call_error(UNKNOWN_ERROR, "new_oscillator()");
    }

    osc->wave = wave;
    osc->fm   = fm;
    osc->am   = am;

    return osc;
}

void free_oscillator(oscillator *osc) {
    free(osc);
}