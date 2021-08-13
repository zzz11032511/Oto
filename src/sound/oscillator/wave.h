#pragma once

#include <stdint.h>

#include "../sound_data.h"

/* 基本波波形 */
enum FUNDAMENTAL_WAVE {
    SINE_WAVE,
    SAWTOOTH_WAVE,
    SQUARE_WAVE,
    TRIANGLE_WAVE,
    PSG_SAW_WAVE,
    PSG_SQUARE_WAVE,
    PSG_TRIANGLE_WAVE,
    WHITE_NOISE,
};

/* 基本波形の標本化周波数の設定. デフォルトは20kHz */
void set_wave_sampling_freq(int32_t fs);

void sine_wave(SOUND s, double freq, uint64_t length);
void sawtooth_wave(SOUND s, double freq, uint64_t length);
void square_wave(SOUND s, double freq, uint64_t length);
void triangle_wave(SOUND s, double freq, uint64_t length);

void psg_sawtooth_wave(SOUND s, double freq, uint64_t length);
void psg_square_wave(SOUND s, double freq, uint64_t length);
void psg_triangle_wave(SOUND s, double freq, uint64_t length);

void white_noise(SOUND s, uint64_t length);
