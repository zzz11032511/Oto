#pragma once

#include <stdint.h>

#define MONO   1
#define STEREO 2

struct track {
    uint64_t length;
    double *wave_data;
    uint64_t t;  // 現在の時間変数

    uint64_t channel;          // MONO or STEREO
    uint64_t samples_per_sec;  // サンプリング周波数[Hz]
    uint64_t bits_per_sample;  // 量子化ビット数
    uint64_t velocity;         // 音量
};
typedef struct track track;

track *new_track(uint64_t length, uint64_t channel, uint64_t samples_per_sec,
                 uint64_t bits_per_sample, uint64_t velocity);
void free_track(track *t);