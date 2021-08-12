#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define PI 3.14159265358979323846

static int32_t SAMPLING_FREQ = 20000;

double sine_wave(int32_t n, double freq, double phase) {
    return sin((2.0 * PI * freq * n / (double)SAMPLING_FREQ) + phase);
}

double sawtooth_wave(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i++) {
        s += 1.0 / (double)i * sine_wave(n, i * freq, 0);
    }
    return s;
}

double square_wave(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i = i + 2) {
        s += 1.0 / i * sine_wave(n, i * freq, 0);
    }
    return s;
}

double triangle_wave(int32_t n, double freq, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i = i + 2) {
        s += 1.0 / i / i * sin(PI * i / 2.0) * sine_wave(n, i * freq, 0);
    }
    return s;    
}

double white_noise(int32_t n, uint32_t arity) {
    double s = 0;
    for (uint32_t i = 1; i <= arity; i++) {
        double phase = (double)rand() / RAND_MAX * 2.0 * PI;
        s += sine_wave(n, (double)i, phase);
    }
    return s;
}

double psg_sawtooth_wave(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    return 1.0 - 2.0 * m / t0;
}

double psg_square_wave(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    if (m < t0 / 2) {
        return 1.0;
    } else {
        return -1.0;
    }
}

double psg_triangle_wave(int32_t n, double freq) {
    int32_t t0 = SAMPLING_FREQ / freq;
    int32_t m = n % t0;

    if (m < t0 / 2) {
        return -1.0 + 4.0 * m / t0;
    } else {
        return  3.0 - 4.0 * m / t0;
    }
}
