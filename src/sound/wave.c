#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#include "sound.h"

#define PI 3.14159265358979323846

static int32_t SAMPLING_FREQ = 40000;

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

/* フェードを掛ける幅 */
static double FADE_RANGE = 0.02;

void sine_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }
    printf("s->ptr : %ld\n", s->ptr);

    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = sine_wave_func(i, freq, 0);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}

void sawtooth_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }

    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = sawtooth_wave_func(i, freq, ARITY);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}

void square_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }
    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = square_wave_func(i, freq, ARITY);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}

void triangle_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }
    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = triangle_wave_func(i, freq, ARITY);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}

void psg_sawtooth_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }

    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = psg_sawtooth_wave_func(i, freq);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}

void psg_square_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }

    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = psg_square_wave_func(i, freq);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}

void psg_triangle_wave(SOUND s, double freq, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }

    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = psg_triangle_wave_func(i, freq);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}

void white_noise(SOUND s, uint64_t length) {
    if ((s->ptr + length) > s->length) {
        length = s->length - s->ptr;
        if (length == 0) return;
    }

    uint64_t i = 0;
    for (i = 0; i < length; i++) {
        s->data[s->ptr + i] = white_noise_func(i, ARITY);
    }

    for (uint64_t n = 0; n < (SAMPLING_FREQ * FADE_RANGE); n++) {
        s->data[s->ptr + n] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
        s->data[s->ptr + i - n - 1] *= (double)n / (SAMPLING_FREQ * FADE_RANGE);
    }

    s->ptr += i;
}
