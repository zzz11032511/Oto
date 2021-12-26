#include <oto/oto.h>
#include <oto/oto_sound.h>

inline static float osc_sine_wave(Playdata *info, uint64_t t, int64_t ch) {
    return sin(2 * PI * info->freq[ch] * t / info->sampling_rate);
}

inline static float osc_saw_wave(Playdata *info, uint64_t t, int64_t ch) {
    uint64_t t0 = info->sampling_rate / info->freq[ch];
    uint64_t m  = t % t0;

    return 1.0 - 2.0 * m / t0;
}

inline static float osc_square_wave(Playdata *info, uint64_t t, int64_t ch) {
    uint64_t t0 = info->sampling_rate / info->freq[ch];
    uint64_t m  = t % t0;

    if (m < t0 / 2) return 1.0;
    else return -1.0;
}

inline static float osc_triangle_wave(Playdata *info, uint64_t t, int64_t ch) {
    uint64_t t0 = info->sampling_rate / info->freq[ch];
    uint64_t m  = t % t0;

    if (m < t0 / 2) return -1.0 + 4.0 * m / t0;
    else return 3.0 - 4.0 * m / t0;
}

static float osc_white_noise(Playdata *info, uint64_t t, int64_t ch) {
    return ((float)rand()) / RAND_MAX;
}

float sound_generate(Playdata *info, uint64_t t, int64_t ch) {
    Sound *sound = info->sound;
    if (IS_NULL(sound)) {
        return osc_sine_wave(info, t, ch);
    }

    switch (sound->oscillator->wave) {
    case SINE_WAVE:
        return osc_sine_wave(info, t, ch);
        break;
    case SAWTOOTH_WAVE:
        return osc_saw_wave(info, t, ch);
        break;
    case SQUARE_WAVE:
        return osc_square_wave(info, t, ch);
        break;
    case TRIANGLE_WAVE:
        return osc_triangle_wave(info, t, ch);
        break;
    case WHITE_NOISE:
        return osc_white_noise(info, t, ch);
        break;
    default:
        return osc_sine_wave(info, t, ch);
    }
}