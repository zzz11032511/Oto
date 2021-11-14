#pragma once

#include <portaudio.h>

#define MAX_POLYPHONIC 8
#define FRAMES_PER_BUFFER 128
#define MAX_VOLUME 100

#define MONO_CH   1
#define STEREO_CH 2

typedef enum {
    SINE_WAVE = 0,
    SAWTOOTH_WAVE,  // PSG
    SQUARE_WAVE,    // PSG
    TRIANGLE_WAVE,  // PSG
    WHITE_NOISE,    // PSG
} basicwave_t;

/* 発振器 */
typedef struct oscillator {
    basicwave_t wave;
    struct oscillator *fm;
    int64_t fm_mod_freq;
    struct oscillator *am;
    int64_t am_mod_freq;
} Oscillator;

/* 音色情報 */
typedef struct {
    Oscillator *oscillator;
    VectorPTR filters;
} Sound;

/* 演奏情報 */
typedef struct {
    Sound *sound;
    uint64_t length;
    float freq[MAX_POLYPHONIC];
    int8_t volume;
} Playdata;

void init_sound_stream();
void terminate_sound_stream();

void update_out_data(Playdata data);