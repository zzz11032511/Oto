#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#include "wave.h"

typedef struct play_data {
    uint32_t note;
    uint32_t length;
    uint8_t  velocity;
    // 使用されるサウンド
    // チャンネル・トラック
} PLAY_DATA;

#define NUMBER_OF_BUFFER 8
#define BUFFER_SIZE 200

void play(double *sound, int32_t sound_length) {
    int16_t out_buffer[NUMBER_OF_BUFFER][BUFFER_SIZE];

    WAVEHDR out_header[NUMBER_OF_BUFFER];
    for (int32_t i = 0; i < NUMBER_OF_BUFFER; i++) {
        memset(&out_header[i], 0, sizeof(WAVEHDR));
    }

    HWAVEOUT out_handle = NULL;
    WAVEFORMATEX wave_format_ex = {
        WAVE_FORMAT_PCM,
        1,          // モノラル
        20000,      // サンプリング周波数
        20000 * 2,  // サンプリング周波数 * 音データの最小単位
        2,          // 音データの最小単位
        16,         // 量子化ビット
        0
    };
    waveOutOpen(&out_handle, 0, &wave_format_ex, 0, 0, CALLBACK_NULL);
    waveOutPause(out_handle);

    int32_t out0 = 0;  // 何番目のバッファを書き込んでるのかを指す
    int32_t out1 = 0;
    int32_t offset = 0;

    int32_t num_of_frame = sound_length / BUFFER_SIZE;
    int32_t frame = 0;
    while (frame < num_of_frame) {
        if (out0 < NUMBER_OF_BUFFER) {
            // バッファへの1frame分の書き込み
            for (int32_t n = 0; n < BUFFER_SIZE; n++) {
                out_buffer[out0][n] = (int16_t)(32760.0 * sound[offset + n]);
            }
            offset += BUFFER_SIZE;
            frame++;

            // バッファの設定
            out_header[out0].lpData = (int8_t *)out_buffer[out0];  // 音データの場所
            out_header[out0].dwBufferLength = BUFFER_SIZE * 2;  // buffer_size * 2[byte](int16)
            out_header[out0].dwFlags = 0;

            // バッファのロック
            waveOutPrepareHeader(out_handle, &out_header[out0], sizeof(WAVEHDR));

            // バッファを出力待ちキューに追加する
            waveOutWrite(out_handle, &out_header[out0], sizeof(WAVEHDR));

            out0++;
            if (out0 == NUMBER_OF_BUFFER) {
                waveOutRestart(out_handle);
            }

        } else if ((out_header[out1].dwFlags & WHDR_DONE) != 0) {
            // 出力バッファのおわりまで音データが再生された
            for (int32_t n = 0; n < BUFFER_SIZE; n++) {
                out_buffer[out1][n] = (int16_t)(32760.0 * sound[offset + n]);
            }
            offset += BUFFER_SIZE;
            frame++;

            waveOutPrepareHeader(out_handle, &out_header[out1], sizeof(WAVEHDR));

            // バッファの設定
            out_header[out1].lpData = (int8_t *)out_buffer[out1];  // 音データの場所
            out_header[out1].dwBufferLength = BUFFER_SIZE * 2;  // buffer_size * 2[byte](int16)
            out_header[out1].dwFlags = 0;

            // バッファのロック
            waveOutPrepareHeader(out_handle, &out_header[out1], sizeof(WAVEHDR));

            // バッファを出力待ちキューに追加する
            waveOutWrite(out_handle, &out_header[out1], sizeof(WAVEHDR));

            out1++;
            if (out1 == NUMBER_OF_BUFFER) out1 = 0;
        }

        // Sleep(1);
    }

    for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++) {
        while ((out_header[out0].dwFlags & WHDR_DONE) == 0) {
            Sleep(1);
        }
    }

    waveOutPause(out_handle);

    for (out0 = 0; out0 < NUMBER_OF_BUFFER; out0++) {
        if ((out_header[out0].dwFlags & WHDR_PREPARED) != 0) {
            waveOutUnprepareHeader(out_handle, &out_header[out0], sizeof(WAVEHDR));
        }
    }

    waveOutClose(out_handle);

    return;
}

int main(void) {
    int32_t sound_length = 20000;
    double sound[sound_length];

    for (int32_t i = 0; i < sound_length; i++) {
        sound[i] = sine_wave(i, 269.292, 0);
    }
    play(sound, sound_length);

    // for (int32_t i = 0; i < sound_length; i++) {
    //     sound[i] = sawtooth_wave(i, 302.270, 44);
    // }
    // play(sound, sound_length);

    // for (int32_t i = 0; i < sound_length; i++) {
    //     sound[i] = square_wave(i, 339.286, 44);
    // }
    // play(sound, sound_length);

    // for (int32_t i = 0; i < sound_length; i++) {
    //     sound[i] = triangle_wave(i, 359.461, 44);
    // }
    // play(sound, sound_length);

    for (int32_t i = 0; i < sound_length; i++) {
        sound[i] = psg_sawtooth_wave(i, 403.482);
    }
    play(sound, sound_length);

    for (int32_t i = 0; i < sound_length; i++) {
        sound[i] = psg_square_wave(i, 452.893);
    }
    play(sound, sound_length);

    for (int32_t i = 0; i < sound_length; i++) {
        sound[i] = psg_triangle_wave(i, 508.355);
    }
    play(sound, sound_length);

    for (int32_t i = 0; i < sound_length; i++) {
        sound[i] = sine_wave(i, 538.584, 0);
    }
    play(sound, sound_length);

    for (int32_t i = 0; i < sound_length; i++) {
        sound[i] = white_noise(i, 10000);
    }
    play(sound, sound_length);

    return 0;
}