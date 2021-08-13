#include <stdio.h>
#include <stdint.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>

#include "sound.h"
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

void play(SOUND s) {
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

    int32_t num_of_frame = s->length / BUFFER_SIZE;
    int32_t frame = 0;
    while (frame < num_of_frame) {
        if (out0 < NUMBER_OF_BUFFER) {
            // バッファへの1frame分の書き込み
            for (int32_t n = 0; n < BUFFER_SIZE; n++) {
                out_buffer[out0][n] = (int16_t)(7000.0 * s->data[offset + n]);
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
                out_buffer[out1][n] = (int16_t)(7000.0 * s->data[offset + n]);
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
    SOUND s = init_sound(20000 * 8);

    sine_wave(s,     269.292, 20000);
    sine_wave(s,     302.270, 20000);
    sine_wave(s,     339.286, 20000);
    sine_wave(s,     359.461, 20000);
    sine_wave(s,     403.482, 20000);
    sine_wave(s,     452.893, 20000);
    sine_wave(s,     508.355, 20000);
    sine_wave(s, 269.292 * 2, 20000);

    play(s);
    
    return 0;
}