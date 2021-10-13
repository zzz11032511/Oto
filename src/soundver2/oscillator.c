#include <stdio.h>
#include <stdlib.h>

#include "oscillator.h"
#include "../util/util.h"
#include "../error/error.h"

oscillator_t*
new_oscillator(basicwave_t wave, oscillator_t *fm, oscillator_t *am) {
    oscillator_t *osc = MYMALLOC(sizeof(oscillator_t));
    if (osc == NULL) {
        call_error(UNKNOWN_ERROR, "new_oscillator()");
    }

    osc->wave = wave;
    osc->fm   = fm;
    osc->am   = am;

    return osc;
}

void
free_oscillator(oscillator_t *osc) {
    free(osc);
}