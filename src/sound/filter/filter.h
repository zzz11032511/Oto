#pragma once

#include <stdint.h>

#include "../sound.h"

enum FILTER_NUMBER {
    FADE_IN,
    FADE_OUT
};

struct filter {
    int32_t filter_num;
    double  param;
    struct filter *next_ftr;
};
typedef struct filter *FILTER;

FILTER new_filter(int32_t filter_num);