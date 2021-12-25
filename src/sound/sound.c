#include <oto/oto.h>
#include <oto/oto_sound.h>

/**
 * osc1 = OSCIL[sine, 0, 0];
 * AAA = SOUND[osc1]
 * 
 * PLAY 500, 1, 1, AAA
 */

Sound *new_sound(Oscillator *osc) {
    Sound *sound = MYMALLOC1(Sound);
    if (IS_NULL(sound)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    sound->oscillator = osc;
    sound->filters = MYMALLOC1(VectorPTR);
    if (IS_NULL(sound->filters)) {
        free(sound);
        oto_error(OTO_INTERNAL_ERROR);
    }

    return sound;
}

Oscillator *new_oscil(basicwave_t wave, basicwave_t fm_wave, float fm_freq) {
    Oscillator *osc = MYMALLOC1(Oscillator);
    if (IS_NULL(osc)) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    osc->wave = wave;
    osc->fm_wave = fm_wave;
    osc->fm_freq = fm_freq;

    return osc;
}

