#pragma once

#include <stdio.h>
#include <stdint.h>

#include "../variable/variable.h"

typedef enum {
    NONE_FILTER = 0,
    FADE,
    FADE_IN,
    FADE_OUT,
    AMP,
    TREMOLO,
    ADSR
} filter_num_t;

struct filter {
    filter_num_t filter_num;
    uint64_t param;
};

typedef struct filter filter;

filter *new_filter(filter_num_t filter_num, uint64_t param);
void free_filter(filter *f);