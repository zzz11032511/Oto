#include <stdio.h>
#include <stdint.h>
#include <portaudio.h>

#include "sound_io.h"
#include "otomath.h"
#include "sound.h"
#include "oscillator.h"
#include "filter.h"
#include "../error/error.h"

static uint64_t sampling_freq = 44100;

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
            ds[ch] = osc_output_wave(
                data->sound->oscillator,
                data->freq[ch],
                data->t,
                data->volume,
                sampling_freq
            );
        }
        d = (ds[0] + ds[1] + ds[2] + ds[3] + ds[4] + ds[5] + ds[6] + ds[7]) / cnt;

        // フィルタリング
        filtering(&d, data->sound, data->t, data->length, sampling_freq);

        clip(&d);
        io_fade(&d, data->t, data->length);
        
        *out++ = d;
        data->t += 1;
    }

    return 0;
}

static PaStream *stream = NULL;
static PaStreamParameters outParam;
static Currentdata data;

void init_sound_io() {
    PaError err = paNoError;
    
    err = Pa_Initialize();
    if (err != paNoError) call_error(SOUND_PLAYER_ERROR);

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

    err = Pa_OpenStream(&stream, NULL, &outParam, sampling_freq,
                        FRAMES_PER_BUFFER, paClipOff, play_callback, &data);
    if (err != paNoError) call_error(SOUND_PLAYER_ERROR);

    err = Pa_StartStream(stream);
    if (err != paNoError) call_error(SOUND_PLAYER_ERROR);
}

void terminate_sound_io() {
    PaError err = paNoError;

    err = Pa_StopStream(stream);
    if (err != paNoError) call_error(SOUND_PLAYER_ERROR);

    err = Pa_CloseStream(stream);
    if (err != paNoError) call_error(SOUND_PLAYER_ERROR);

    err = Pa_Terminate();
    if (err != paNoError) call_error(SOUND_PLAYER_ERROR);
}