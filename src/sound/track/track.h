#pragma once

#include <stdint.h>

/* 音データを保存する構造体 */
struct track_t {
    uint64_t length;
    double *data;
    uint64_t ptr;
};
typedef struct track_t *TRACK;

/* track構造体を新しく作る */
TRACK new_track(uint64_t length);

void free_track(TRACK s);

/* カーソルを先頭に戻す */
void rewind_track(TRACK s);

/* 現在位置からoffset分移動する */
void seek_track(TRACK s, uint64_t offset);
