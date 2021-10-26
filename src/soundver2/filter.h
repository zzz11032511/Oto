#pragma once

#include <stdio.h>
#include <stdint.h>

#define OTO_FILTER inline void

OTO_FILTER clip(float *d);
OTO_FILTER fade_in(float *d, uint64_t t, uint64_t length, double time);
OTO_FILTER fade_out(float *d, uint64_t t, uint64_t length, double time);
OTO_FILTER fade(float *d, uint64_t t, uint64_t length, double start, double end);
OTO_FILTER io_fade(float *d, uint64_t t, uint64_t length);


typedef struct {
    filter_num_t filter_num;
    uint64_t param;
} Filter;

typedef enum {
    NONE_FILTER = 0,
    CLIP,
    FADE_IN,
    FADE_OUT,
    FADE,
    AMP,
    TREMOLO
} filter_num_t;

Filter *new_filter(filter_num_t filter_num, uint64_t param);

void filtering(float *d, Sound *s, uint64_t t, uint64_t length,
               uint64_t sampling_freq);