#pragma once

#include <stdint.h>

#define MONO   1
#define STEREO 2

/* 音データを保存する構造体 */
struct track_t {
    uint64_t length;
    double *data;
    uint64_t ptr;  // 音データをここまで書き込んだ

    uint64_t channel;  // MONO or STEREO
    uint64_t samples_per_sec;  // サンプリング周波数[Hz]
    uint64_t bits_per_sample;  // 量子化ビット数
};
typedef struct track_t *TRACK;

TRACK new_track(uint64_t length, uint64_t channel,
                uint64_t samples_per_sec, uint64_t bits_per_sample);

void free_track(TRACK s);