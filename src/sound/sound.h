#pragma once

#include <stdint.h>

typedef struct sound_t {
    uint64_t length;
    double *data;
    uint64_t ptr;
} SOUND_OBJECT;
typedef SOUND_OBJECT *SOUND;

SOUND init_sound(uint64_t length);
void free_sound(SOUND s);
void rewind_sound(SOUND s);
