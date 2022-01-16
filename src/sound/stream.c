#include <oto/oto.h>
#include <oto/oto_sound.h>

// PRINTWAV, EXPORT用
float *databuf = NULL;

static PaStreamParameters out_param;
static void init_stream_param() {
    out_param.channelCount = MONO_CH;
    out_param.sampleFormat = paFloat32;
    out_param.device = Pa_GetDefaultOutputDevice();
    out_param.suggestedLatency =
        Pa_GetDeviceInfo(out_param.device)->defaultLowOutputLatency;
    out_param.hostApiSpecificStreamInfo = NULL;
}

/* 現在の演奏情報 */
typedef struct {
    uint64_t t;
    Playdata info;
    bool print_flag;
    bool safety_flag;
} Currentdata;
Currentdata out_data;

static void init_out_data(int64_t sampling_rate, bool print_flag, bool safety_flag) {
    out_data.info.sound = NULL;
    out_data.info.length = 0;
    out_data.info.volume = 0;
    out_data.t = 0;
    for (uint64_t i = 0; i < MAX_POLYPHONIC; i++) {
        out_data.info.freq[i] = 1;
    }
    out_data.info.sampling_rate = sampling_rate;
    out_data.print_flag = print_flag;
    out_data.safety_flag = safety_flag;
}

static bool stream_active_flag = false;
bool is_stream_active() {
    return stream_active_flag;
}

void set_stream_active_flag(bool b) {
    stream_active_flag = b;
}

void write_out_data(Playdata data, bool print_flag) {
    out_data.t = 0;
    out_data.info.sound = data.sound;
    out_data.info.length = data.length;
    for (uint64_t i = 0; i < MAX_POLYPHONIC; i++) {
        out_data.info.freq[i] = data.freq[i];
    }
    out_data.info.volume = data.volume;
    out_data.print_flag = print_flag;
}

static double FADE_RANGE = 0.05;
static int play_callback(const void *inputBuffer,
                         void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo *timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData)
{
    Currentdata *data = (Currentdata *)userData;
    
    // ここに出力データを書き込む
    float *out = (float *)outputBuffer;
    float d = 0;

    for (uint64_t i = 0; i < framesPerBuffer; i++) {
        if (data->t > data->info.length) {
            *out++ = 0;
            continue;
        }

        d = ((float)data->info.volume / 100) * sound_generate(&(data->info), data->t, 0);
        d = filtering(d, &data->info, data->t);

        /* フェード処理 */
        if (data->t < (FADE_RANGE * data->info.length)) {
            d *= data->t / (FADE_RANGE * data->info.length);
        } else if ((data->info.length - data->t) < (FADE_RANGE * data->info.length)) {
            d *= (data->info.length - data->t) / (FADE_RANGE * data->info.length);
        }

        if (data->safety_flag) {
            if (d > 0.8)  d = 0.8;
            if (d < -0.8) d = -0.8;
        }

        if (data->print_flag) {
            databuf[data->t] = d;
            *out++ = d;
        } else {
            *out++ = d;
        }

        data->t += 1;
    }

    if (data->t > data->info.length) {
        stream_active_flag = false;
        data->print_flag = false;
    }

    return 0;
}

static PaStream *stream;
void init_sound_stream(Status *status) {
    PaError err = paNoError;

    err = Pa_Initialize();
    if (err != paNoError) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    init_stream_param();
    init_out_data(status->sampling_rate, false, status->safety_flag);
    FADE_RANGE = status->fade_range;

    err = Pa_OpenStream(&stream, NULL, &out_param,
                        (float)status->sampling_rate,
                        FRAMES_PER_BUFFER, paClipOff, play_callback, &out_data);
    if (err != paNoError) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        oto_error(OTO_INTERNAL_ERROR);
    }
}

void terminate_sound_stream() {
    if (IS_NOT_NULL(databuf)) {
        free(databuf);
    }
    PaError err = paNoError;

    if (!Pa_IsStreamStopped(stream)) {
        err = Pa_StopStream(stream);
        if (err != paNoError) {
            oto_error(OTO_INTERNAL_ERROR);
        }
    }

    err = Pa_CloseStream(stream);
    if (err != paNoError) {
        oto_error(OTO_INTERNAL_ERROR);
    }

    err = Pa_Terminate();
    if (err != paNoError) {
        oto_error(OTO_INTERNAL_ERROR);
    }
}
