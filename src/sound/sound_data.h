#pragma once

#include <stdint.h>

/* 音データを保存する構造体 */
typedef struct sound_t {
    uint64_t length;
    double *data;
    uint64_t ptr;
} SOUND_OBJECT;
typedef SOUND_OBJECT *SOUND;

/* sound構造体を新しく作る */
SOUND new_sound(uint64_t length);

void free_sound(SOUND s);

/* カーソルを先頭に戻す */
void rewind_sound(SOUND s);

/* 現在位置からoffset分移動する */
void seek_sound(SOUND s, uint64_t offset);
