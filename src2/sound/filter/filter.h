#pragma once

typedef enum {
    NONE_FILTER = 0,
    FADE_IN,
    FADE_OUT,
    AMP,
    TREMOLO,
    ADSR,
    LOW_PASS,
    HIGH_PASS,
    VIBRATO,
    WAH,
    DELAY,
    REVERB,
} filter_num_t;

struct filter {
    filter_num_t filter_num;
    uint64_t param;
};

typedef struct filter *FILTER;

void init_filter();