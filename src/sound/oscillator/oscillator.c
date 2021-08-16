#include "oscillator.h"

#include <stdlib.h>
#include <stdint.h>

#include "wave.h"
#include "../sound_io.h"
#include "../track/track.h"
#include "../../utils/util.h"

/* フェードを掛ける幅 */
static double FADE_RANGE = 0.05;

void write_wave(int32_t wave, 
                TRACK t, double freq, uint64_t length,
                int32_t sampling_freq, bool_t fade_flag, bool_t seek_cursol_flag) {
    set_wave_sampling_freq(sampling_freq);
    
    if ((t->ptr + length) > t->length) {
        // 音データの長さを超えてしまう場合
        length = t->length - t->ptr;
        if (length == 0) return;
    }

    switch (wave) {
    case SINE_WAVE:
        sine_wave(t, freq, length);
        break;
    case SAWTOOTH_WAVE:
        sawtooth_wave(t, freq, length);
        break;
    case SQUARE_WAVE:
        square_wave(t, freq, length);
        break;
    case TRIANGLE_WAVE:
        triangle_wave(t, freq, length);
        break;
    case PSG_SAW_WAVE:
        psg_sawtooth_wave(t, freq, length);
        break;
    case PSG_SQUARE_WAVE:
        psg_square_wave(t, freq, length);
        break;
    case PSG_TRIANGLE_WAVE:
        psg_triangle_wave(t, freq, length);
        break;
    case WHITE_NOISE:
        white_noise(t, freq);
        break;
    }

    /* フェード処理 */
    if (fade_flag) {
        for (uint64_t n = 0; n < (sampling_freq * FADE_RANGE); n++) {
            t->data[t->ptr + n] *= (double)n / (sampling_freq * FADE_RANGE);
            t->data[t->ptr + length - n - 1] *= (double)n / (sampling_freq * FADE_RANGE);
        }        
    }

    if (seek_cursol_flag) t->ptr += length;
}
