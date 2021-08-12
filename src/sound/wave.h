#pragma once

#include <stdint.h>

double sine_wave(int32_t n, double freq, double phase); // OK
double sawtooth_wave(int32_t n, double freq, uint32_t arity); // NG
double square_wave(int32_t n, double freq, uint32_t arity); // NG
double triangle_wave(int32_t n, double freq, uint32_t arity); // NG

double white_noise(int32_t n, uint32_t arity); // OK?

double psg_sawtooth_wave(int32_t n, double freq); // OK?
double psg_square_wave(int32_t n, double freq); // OK?
double psg_triangle_wave(int32_t n, double freq); // OK