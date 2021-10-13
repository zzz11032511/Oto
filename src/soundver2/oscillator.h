#pragma once

#include <stdint.h>

typedef enum {
    SINE_WAVE = 0,
    SAWTOOTH_WAVE,  // PSG
    SQUARE_WAVE,    // PSG
    TRIANGLE_WAVE,  // PSG
    WHITE_NOISE,    // PSG
} basicwave_t;

struct oscillator {
    basicwave_t wave;
    struct oscillator *fm;  // モジュレータ周波数(NULLなら0)
    struct oscillator *am;  // モジュレータ振幅(NULLなら0)
};

typedef struct oscillator oscillator;

oscillator *new_oscillator(basicwave_t wave, oscillator *fm, oscillator *am);
void free_oscillator(oscillator *osc);