#pragma once

#include <stdint.h>

#include "../track/track.h"

/* 基本波形の標本化周波数の設定. デフォルトは20kHz */
void set_wave_sampling_freq(int32_t fs);

void sine_wave(TRACK t, double freq, uint64_t length);
void sawtooth_wave(TRACK t, double freq, uint64_t length);
void square_wave(TRACK t, double freq, uint64_t length);
void triangle_wave(TRACK t, double freq, uint64_t length);

void psg_sawtooth_wave(TRACK t, double freq, uint64_t length);
void psg_square_wave(TRACK t, double freq, uint64_t length);
void psg_triangle_wave(TRACK t, double freq, uint64_t length);

void white_noise(TRACK t, uint64_t length);
