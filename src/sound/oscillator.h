#pragma once

#include <stdint.h>

typedef enum {
    SINE_WAVE = 0,
    SAWTOOTH_WAVE,  // PSG
    SQUARE_WAVE,    // PSG
    TRIANGLE_WAVE,  // PSG
    WHITE_NOISE,    // PSG
} basicwave_t;

typedef struct Oscillator {
    basicwave_t wave;
    struct Oscillator *fm;  // モジュレータ周波数(NULLなら0)
    struct Oscillator *am;  // モジュレータ振幅(NULLなら0)
} Oscillator;

Oscillator *new_oscillator(basicwave_t wave, Oscillator *fm, Oscillator *am);
void free_oscillator(Oscillator *osc);

float osc_output_wave(Oscillator *osc, float freq, uint64_t t, uint8_t volume,
                      uint64_t sampling_freq);