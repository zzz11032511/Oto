#include "oscillator.h"

#include <stdlib.h>
#include <stdint.h>

#include "wave.h"
#include "../sound_io.h"
#include "../sound_data.h"
#include "../../utils/util.h"

/* フェードを掛ける幅 */
static double FADE_RANGE = 0.05;

void write_wave(int32_t wave, 
                 SOUND s, double freq, uint64_t length,
                 int32_t sampling_freq, bool_t fade_flag, bool_t seek_cursol_flag) {
    set_wave_sampling_freq(sampling_freq);
    
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
