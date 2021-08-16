#pragma once

#include <stdint.h>

struct sound_t {
    int32_t wave;
};
typedef struct sound_t *SOUND;

SOUND new_sound(int32_t wave);