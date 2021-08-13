#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "sound.h"

#define PI 3.14159265358979323846

static int32_t SAMPLING_FREQ = 20000;

double sine_wave_func(int32_t n, double freq, double phase) {
    return sin((2.0 * PI * freq * n / (double)SAMPLING_FREQ) + phase);
}

double sawtooth_wave_func(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i++) {
        s += 1.0 / (double)i * sine_wave_func(n, i * freq, 0);
    }
    return s;
}

double square_wave_func(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i = i + 2) {
        s += 1.0 / i * sine_wave_func(n, i * freq, 0);
    }
    return s;
}

double triangle_wave_func(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i = i + 2) {
        s += 1.0 / i / i * sin(PI * i / 2.0) * sine_wave_func(n, i * freq, 0);
    }
    return s;    
}

double psg_sawtooth_wave_func(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    return 1.0 - 2.0 * m / t0;
}

double psg_square_wave_func(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    if (m < t0 / 2) {
        return 1.0;
    } else {
        return -1.0;
    }
}

double psg_triangle_wave_func(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    if (m < t0 / 2) {
        return -1.0 + 4.0 * m / t0;
    } else {
        return  3.0 - 4.0 * m / t0;
    }
}

double white_noise_func(int32_t n, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i++) {
        double phase = (double)rand() / RAND_MAX * 2.0 * PI;
        s += sine_wave_func(n, (double)i, phase);
    }
    return s;
}

/* 正弦波を足し合わせる回数 */
static int32_t ARITY = 50;

void sine_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
    }
    uint64_t i = 0;
    while (i < length) {
        s->data[i + s->ptr] = sine_wave_func(i, freq, 0);
        i++;
    }
    s->ptr += i;
}

void sawtooth_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
    }
    uint64_t i = 0;
    while (i < length) {
        s->data[i + s->ptr] = sawtooth_wave_func(i, freq, ARITY);
        i++;
    }
    s->ptr += i;
}

void triangle_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
    }
    uint64_t i = 0;
    while (i < length) {
        s->data[i + s->ptr] = triangle_wave_func(i, freq, ARITY);
        i++;
    }
    s->ptr += i;
}

void psg_sawtooth_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
    }
    uint64_t i = 0;
    while (i < length) {
        s->data[i + s->ptr] = psg_sawtooth_wave_func(i, freq);
        i++;
    }
    s->ptr += i;
}

void psg_square_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
    }
    uint64_t i = 0;
    while (i < length) {
        s->data[i + s->ptr] = psg_square_wave_func(i, freq);
        i++;
    }
    s->ptr += i;
}

void psg_triangle_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
    }
    uint64_t i = 0;
    while (i < length) {
        s->data[i + s->ptr] = psg_triangle_wave_func(i, freq);
        i++;
    }
    s->ptr += i;
}

void white_noise(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
    }
    uint64_t i = 0;
    while (i < length) {
        s->data[i + s->ptr] = white_noise_func(i, ARITY);
        i++;
    }
    s->ptr += i;
}
