#include <stdio.h>
#include <stdlib.h>

#include "oscillator.h"
#include "otomath.h"
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

static float osc_sine_wave(float freq, uint64_t t, uint8_t volume, float phase,
                           uint64_t sampling_freq) {
    return volume * sin(2 * PI * freq * t / sampling_freq + phase);
}

static float osc_saw_wave(float freq, uint64_t t, uint8_t volume,
                          uint64_t sampling_freq) {
    uint64_t t0 = sampling_freq / freq;
    uint64_t m  = t % t0;

    return 1.0 - 2.0 * m / t0;
}

static float osc_square_wave(float freq, uint64_t t, uint8_t volume,
                             uint64_t sampling_freq) {
    uint64_t t0 = sampling_freq / freq;
    uint64_t m  = t % t0;

    if (m < t0 / 2) return 1.0;
    else return -1.0;
}

static float osc_triangle_wave(float freq, uint64_t t, uint8_t volume,
                               uint64_t sampling_freq) {
    uint64_t t0 = sampling_freq / freq;
    uint64_t m  = t % t0;

    if (m < t0 / 2) return -1.0 + 4.0 * m / t0;
    else return 3.0 - 4.0 * m / t0;
}

static float osc_white_noise(float freq, uint64_t t, uint8_t volume,
                             uint64_t sampling_freq) {
    float d = 0;
    for (uint64_t i = 1; i < 30; i++) {
        float phase = (float)rand() / RAND_MAX * 2.0 * PI;
        d += osc_sine_wave(freq, t, volume, phase, sampling_freq);
    }
    return d;
}


static float osc_output_wave_sub(Oscillator *osc, float freq, uint64_t t,
                                 uint8_t volume, uint64_t sampling_freq) {
    float d = 0;

    switch (osc->wave) {
    case SINE_WAVE:
        d = osc_sine_wave(freq, t, volume, 0, sampling_freq);
        break;
    case SAWTOOTH_WAVE:
        d = osc_saw_wave(freq, t, volume, sampling_freq);
        break;
    case SQUARE_WAVE:
        d = osc_square_wave(freq, t, volume, sampling_freq);
        break;
    case TRIANGLE_WAVE:
        d = osc_triangle_wave(freq, t, volume, sampling_freq);
        break;
    case WHITE_NOISE:
        d = osc_white_noise(freq, t, volume, sampling_freq);
        break;
    default:
        call_error(UNKNOWN_ERROR, "osc_output_wave()");
    }    
}

float osc_output_wave(Oscillator *osc, float freq, uint64_t t, uint8_t volume,
                      uint64_t sampling_freq) {
    if (osc->fm != NULL) {
        // fm
    }
    if (osc->am != NULL) {
        // am
    }

    return osc_output_wave_sub(osc, freq, t, volume, sampling_freq);
}