#pragma once

#include <stdio.h>
#include <stdint.h>

#define OTO_FILTER inline void

OTO_FILTER clip(float *d);
OTO_FILTER fade_in(float *d, uint64_t t, uint64_t length, double time);
OTO_FILTER fade_out(float *d, uint64_t t, uint64_t length, double time);
OTO_FILTER fade(float *d, uint64_t t, uint64_t length, double start, double end);

// #include "../variable/variable.h"

// typedef enum {
//     NONE_FILTER = 0,
//     FADE,
//     FADE_IN,
//     FADE_OUT,
//     AMP,
//     TREMOLO,
//     ADSR
// } filter_num_t;

// typedef struct {
//     filter_num_t filter_num;
//     uint64_t param;
// } filter;

// filter *new_filter(filter_num_t filter_num, uint64_t param);
// void free_filter(filter *f);