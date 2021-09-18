#pragma once

#include <stdint.h>

/* 音データを保存する構造体 */
struct track_t {
    uint64_t length;
    double *data;
    uint64_t ptr;
};
typedef struct track_t *TRACK;

TRACK new_track(uint64_t length);

void free_track(TRACK s);