#pragma once

#include <stdint.h>

#include "../utils/util.h"

typedef struct {
    int32_t  sampling_freq;
    int32_t  bits;
    uint64_t length;
    double *s;
}MONO_PCM;

void mono_wave_write(MONO_PCM *pcm, str_t filename);