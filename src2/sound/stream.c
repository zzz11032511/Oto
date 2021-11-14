#include <oto.h>
#include <oto_sound.h>

static uint64_t sampling_freq = 44100;

static PaStreamParameters out_param;
static void init_stream_param() {
    out_param.channelCount = MONO_CH;
    out_param.sampleFormat = paFloat32;
    out_param.device = Pa_GetDefaultOutputDevice();
    out_param.suggestedLatency =
        Pa_GetDeviceInfo(out_param.device)->defaultLowOutputLatency;
    out_param.hostApiSpecificStreamInfo = NULL;
}

typedef struct {
    Sound *sound;
    uint64_t length;
    uint64_t t;
    float freq[MAX_POLYPHONIC];
    int8_t volume; 
} Currentdata;
Currentdata out_data;

static void init_out_data() {
    out_data.sound = NULL;
    out_data.length = 0;
    out_data.volume = 0;
    out_data.t = 0;
    for (uint64_t i = 0; i < MAX_POLYPHONIC; i++) {
        out_data.freq[i] = 1;
    }
}

void update_out_data(Playdata data) {
    out_data.sound = data.sound;
    out_data.length = data.length;
    out_data.t = 0;
    for (uint64_t i = 0; i < MAX_POLYPHONIC; i++) {
        out_data.freq[i] = data.freq[i];
    }
    out_data.volume = data.volume;
}

#define FADE_RANGE 0.15

static int play_callback(const void *inputBuffer,
                         void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    Currentdata *data = (Currentdata *)userData;
    float volume = (float)data->volume / MAX_VOLUME;
    float ds[MAX_POLYPHONIC] = {0};
    
    // ここに出力データを書き込む
    float *out = (float *)outputBuffer;
    float d = 0;

    for (uint64_t i = 0; i < framesPerBuffer; i++) {
        if (data->t >= data->length) {
            *out++ = 0;
            data->t += 1;
            continue;
        }

        // uint64_t num_of_sounds = 0;
        // for (uint64_t i = 0; i < MAX_POLYPHONIC; i++) {
        //     ds[i] = sin(2 * PI * data->freq[i] * data->t / sampling_freq);
        // }

        // for (uint64_t i = 0; i < MAX_POLYPHONIC; i++) {
        //     d += ds[i];
        // }
        // d = d / MAX_POLYPHONIC;

        d = sin(2 * PI * data->freq[0] * data->t / sampling_freq);

        /* フェード処理 */
        if (data->t < (uint64_t)(FADE_RANGE * data->length)) {
            d *= data->t / (FADE_RANGE * data->length);
        } else if ((data->length - data->t) < (uint64_t)(FADE_RANGE * data->length)) {
            d *= (data->length - data->t) / (FADE_RANGE * data->length);
        }

        *out++ = d;

        data->t += 1;
    }

    return 0;
}

static PaStream *stream;
void init_sound_stream() {
    PaError err = paNoError;

    err = Pa_Initialize();
    if (err != paNoError) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    init_stream_param();
    init_out_data();

    err = Pa_OpenStream(&stream, NULL, &out_param, sampling_freq,
                        FRAMES_PER_BUFFER, paClipOff, play_callback, &out_data);
    if (err != paNoError) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }
}

void terminate_sound_stream() {
    PaError err = paNoError;

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }

    err = Pa_Terminate();
    if (err != paNoError) {
        oto_error_exit(OTO_INTERNAL_ERROR);
    }
}