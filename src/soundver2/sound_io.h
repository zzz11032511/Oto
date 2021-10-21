#pragma once

#include <stdint.h>

/* 最大同時発音数 */
#define MAX_POLYPHONIC 8

/* 音量の最大値 */
#define MAX_VOLUME 100

void set_sampling_freq(uint64_t f);
void set_fade_range(double range);

/* 必ずterminate_sound_io()を終わるタイミングで使用する */
void init_sound_io();
void terminate_sound_io();