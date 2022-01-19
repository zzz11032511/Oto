#pragma once

#include <math.h>
#include <unistd.h>
#include <portaudio.h>

// #define MAX_POLYPHONIC 8
#define MAX_POLYPHONIC 6
#define FRAMES_PER_BUFFER 128
#define MAX_VOLUME 100

#define MONO_CH   1
#define STEREO_CH 2

#define PI 3.14159265358979323846
#define M_SQRT2   1.41421356237309504880
#define M_SQRT1_2 0.70710678118654752440

typedef enum {
    NO_WAVE = 0,
    SINE_WAVE,
    SAWTOOTH_WAVE,  // PSG
    SQUARE_WAVE,    // PSG
    TRIANGLE_WAVE,  // PSG
    WHITE_NOISE,    // PSG
} basicwave_t;

/* 発振器 */
typedef struct oscillator {
    basicwave_t wave;
    // struct oscillator *fm;
    // struct oscillator *am;

    // 妥協したFM音源
    basicwave_t fm_wave;
    float fm_freq;
} Oscillator;

/* 音色情報 */
typedef struct {
    Oscillator *oscillator;
    VectorPTR *filters;
} Sound;

/* 演奏情報 */
typedef struct {
    Sound *sound;
    uint64_t sound_num;
    uint64_t length;
    float freq[MAX_POLYPHONIC];
    int8_t volume;
    int64_t sampling_rate;
} Playdata;

#define FILTER_ARG_SIZE 10

/* フィルタ */
typedef struct {
    filtercode_t num;
    Var *args[FILTER_ARG_SIZE];
} Filter;

void init_sound_stream(Status *status);
void terminate_sound_stream();
void init_filter(VectorPTR *var_list);

Filter *new_filter(filtercode_t fc);
Oscillator *new_oscil(basicwave_t wave, basicwave_t fm_wave, float fm_freq);
Sound *new_sound(Oscillator *osc);

bool is_stream_active();
void set_stream_active_flag(bool b);

// printwav, export命令用に音データを保存するバッファ
extern float *databuf;
void write_out_data(Playdata data, bool print_flag, bool fade_flag);

float sound_generate(Playdata *info, uint64_t t, int64_t ch);
float filtering(float data, Playdata *info, uint64_t t);
