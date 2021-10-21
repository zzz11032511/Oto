#pragma once

#include <stdint.h>

#include "sound.h"

#define MONO   1
#define STEREO 2

/* 演奏情報 */
typedef struct {
    Sound *sound;
    double freq;
    double length;
    uint8_t velocity;
} note_data;

struct track {
    Sound *sound;

    note_data *note;
    size_t max_of_notes;
    size_t num_of_notes;
    
    uint32_t bpm;
    uint32_t beat;

    uint64_t t;  // 時変数

    uint8_t channel;
    uint64_t sample_per_sec;
    uint64_t bits_per_sample;
    uint8_t volume;
};
typedef struct track track;