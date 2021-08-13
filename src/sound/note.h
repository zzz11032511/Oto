#pragma once

#include "../utils/util.h"

struct note {
    str_t note_name;
    int32_t octave;
    double note_frequency;
};

const struct note note_list[] = {
    {"C",  4, 269.292},
    {"C#", 4, 285.305},
    {"D",  4, 302.270},
    {"D#", 4, 320.244},
    {"E",  4, 339.286},
    {"F",  4, 359.461},
    {"F#", 4, 380.836},
    {"G",  4, 403.482},
    {"G#", 4, 427.474},
    {"A",  4, 452.893},
    {"A#", 4, 479.823},
    {"B",  4, 508.355},
}
