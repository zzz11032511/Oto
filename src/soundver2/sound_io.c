#include <stdio.h>
#include <stdint.h>
#include <portaudio.h>

#include "sound_io.h"
#include "otomath.h"
#include "sound.h"
#include "filter.h"

static uint64_t fs = 44100;
void set_sampling_freq(uint64_t f) {
    fs = f;
}

static double fade_range = 0.05;
void set_fade_range(double range) {
    fade_range = range;
}

static uint64_t frames_per_buffer = 128;

/* 現在の出力音声情報 */
typedef struct {
    Sound *sound;
    uint64_t length;
    uint64_t t;

    float freq[MAX_POLYPHONIC];
    uint8_t volume;
} Currentdata;

static int play_callback(const void *inputBuffer,
                         void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    Currentdata *data = (Currentdata *)userData;
    float *out = (float *)outputBuffer;

    float volume = (float)data->volume / MAX_VOLUME;
    float ds[MAX_POLYPHONIC] = {0};
    float d = 0;

    for (uint64_t i = 0; i < framesPerBuffer; i++) {
        if (data->t >= data->length) {
            *out++ = 0;
            data->t += 1;
            continue;
        }

        // オシレータ
        uint64_t cnt = 1;
        for (uint64_t ch = 0; ch < MAX_POLYPHONIC; ch++) {
            ds[ch] = volume * sin(2 * PI * data->freq[ch] * data->t / fs);
        }
        d = (ds[0] + ds[1] + ds[2] + ds[3] + ds[4] + ds[5] + ds[6] + ds[7]) / cnt;

        // フィルタリング

        clip(&d);
        fade(&d, data->t, data->length, fade_range, fade_range);
        
        *out++ = d;
        data->t += 1;
    }

    return 0;
}

static PaStream *stream = NULL;
static PaStreamParameters outParam;
static Currentdata data;

void init_sound_io() {
    Pa_Initialize();

    outParam.channelCount = 1;
    outParam.sampleFormat = paFloat32;
    outParam.device = Pa_GetDefaultOutputDevice();
    outParam.suggestedLatency = 
        Pa_GetDeviceInfo(outParam.device)->defaultLowOutputLatency;
    outParam.hostApiSpecificStreamInfo = NULL;

    data.length = 0;
    data.volume = 0;
    data.t = 0;
    for (uint64_t i = 0; i < MAX_POLYPHONIC; i++) {
        data.freq[i] = 1;
    }

    Pa_OpenStream(&stream, NULL, &outParam, fs, frames_per_buffer, paClipOff,
                  play_callback, &data);
    Pa_StartStream(stream);
}

void terminate_sound_io() {
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
}