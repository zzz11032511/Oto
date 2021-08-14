#include "oscillator.h"

#include <stdlib.h>
#include <stdint.h>

#include "wave.h"
#include "../sound_io.h"
#include "../sound_data.h"
#include "../../utils/util.h"

/* フェードを掛ける幅 */
static double FADE_RANGE = 0.02;

void write_wave(int32_t wave, 
                 SOUND s, double freq, uint64_t length,
                 int32_t sampling_freq, bool_t fade_flag, bool_t seek_cursol_flag) {
    if ((s->ptr + length) > s->length) {
        // 音データの長さを超えてしまう場合
        length = s->length - s->ptr;
        if (length == 0) return;
    }

    switch (wave) {
    case SINE_WAVE:
        sine_wave(s, freq, length);
        break;
    case SAWTOOTH_WAVE:
        sawtooth_wave(s, freq, length);
        break;
    case SQUARE_WAVE:
        square_wave(s, freq, length);
        break;
    case TRIANGLE_WAVE:
        triangle_wave(s, freq, length);
        break;
    case PSG_SAW_WAVE:
        psg_sawtooth_wave(s, freq, length);
        break;
    case PSG_SQUARE_WAVE:
        psg_square_wave(s, freq, length);
        break;
    case PSG_TRIANGLE_WAVE:
        psg_triangle_wave(s, freq, length);
        break;
    case WHITE_NOISE:
        white_noise(s, freq);
        break;
    }

    /* フェード処理 */
    if (fade_flag) {
        for (uint64_t n = 0; n < (sampling_freq * FADE_RANGE); n++) {
            s->data[s->ptr + n] *= (double)n / (sampling_freq * FADE_RANGE);
            s->data[s->ptr + length - n - 1] *= (double)n / (sampling_freq * FADE_RANGE);
        }        
    }

    if (seek_cursol_flag) s->ptr += length;
}

// int main(void) {
//     // gcc .\oscillator.c ..\sound_data.c ..\sound_io.c .\wave.c -O2 -lwinmm
//     int32_t fs = 20000;

//     SOUND s = new_sound(fs * 8);
//     write_sound(TRIANGLE_WAVE, s, 269.292 * 2, fs * 2, fs, true, false);
//     write_sound(TRIANGLE_WAVE, s, 339.286 * 2, fs * 2, fs, true, false);
//     write_sound(TRIANGLE_WAVE, s, 403.482 * 2, fs * 2, fs, true, true);

//     write_sound(TRIANGLE_WAVE, s, 285.305 * 2, fs * 2, fs, true, false);
//     write_sound(TRIANGLE_WAVE, s, 359.461 * 2, fs * 2, fs, true, false);
//     write_sound(TRIANGLE_WAVE, s, 452.893 * 2, fs * 2, fs, true, true);

//     write_sound(SQUARE_WAVE,       s, 269.292 * 2, fs, fs, true, true);
//     write_sound(TRIANGLE_WAVE,     s, 302.270 * 2, fs, fs, true, true);
//     write_sound(PSG_SQUARE_WAVE,   s, 339.286 * 2, fs, fs, true, true);
//     write_sound(PSG_TRIANGLE_WAVE, s, 359.461 * 2, fs, fs, true, true);

//     play_sound(s, fs);
    
//     return 0;
// }
