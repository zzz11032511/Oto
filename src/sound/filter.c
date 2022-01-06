#include <oto/oto.h>
#include <oto/oto_sound.h>

const struct init_define_filters def_filters[] = {
    {"CLIP",        4, CLIP,        0},
    {"FADE_IN",     7, FADE_IN,     1},
    {"FADE_OUT",    8, FADE_OUT,    1},
    {"FADE",        4, FADE,        2},
    {"AMP",         3, AMP,         1},
    {"TREMOLO",     7, TREMOLO,     2},
    {"DETUNE",      6, DETUNE,      1},
    {"CHOP",        4, CHOP,        1},
    {"LPF",         3, LPF,         1},
    {"HPF",         3, HPF,         1},
    {"WAH",         3, WAH,         3},
    {"RADIO",       5, RADIO,       0}
};

Filter *new_filter(filtercode_t fc) {
    if (!(0 <= fc && fc < FILTER_NUM)) {
        return NULL;
    }
    
    Filter *filter = MYMALLOC1(Filter);
    if (IS_NULL(filter)) {
        return NULL;
    }

    filter->num = fc;
    for (int64_t i = 0; i < FILTER_ARG_SIZE; i++) {
        filter->args[i] = NULL;
    }

    return filter;
}

void init_filter(VectorPTR *var_list) {
    int64_t filters_num = GET_ARRAY_LENGTH(def_filters);
    int64_t i = 0;

    while (i < filters_num) {
        tokencode_t tc = allocate_tc(
            def_filters[i].s, 
            def_filters[i].sl,
            TK_TY_VARIABLE,
            var_list
        );

        Var *filter_var = (Var *)(var_list->data[tc]);
        filter_var->type = TY_FILTER;
        filter_var->value.p = NULL;

        i++;
    }
}

inline static float clip(float d) {
    if (d > 1) {
        return 1;
    } else if (d < -1) {
        return -1;
    }
    return d;
}

inline static float fade_in(float d, Playdata *info, uint64_t t, double fade_time) {
    fade_time = fade_time * info->sampling_rate;
    if (t < fade_time) {
        return d * (t / fade_time);
    }
    return d;
}

inline static float fade_out(float d, Playdata *info, uint64_t t, double fade_time) {
    fade_time = fade_time * info->sampling_rate;
    if ((info->length - t) < fade_time) {
        return d * ((info->length - t) / fade_time);
    }
    return d;
}

inline static float fade(float d, Playdata *info, uint64_t t, double fadein_time, double fadeout_time) {
    d = fade_in(d, info, t, fadein_time);
    d = fade_out(d, info, t, fadeout_time);
    return d;
}

inline static float amp(float d, Playdata *info, uint64_t t, double gain) {
    d *= gain;
    d = clip(d);
    return d;
}

inline static float tremolo(float d, Playdata *info, uint64_t t, double depth, double speed) {
    d *= depth * sin((2 * PI * speed * t) / info->sampling_rate);
    return d;
}

inline static float detune(float d, Playdata *info, uint64_t t, double depth) {
    float data = d;
    for (int64_t ch = 0; ch < MAX_POLYPHONIC; ch++) {
        float org = info->freq[ch];
        info->freq[ch] += depth;
        data += ((float)info->volume / 100) * sound_generate(info, t, ch);
        info->freq[ch] = org;
    }
    data /= (MAX_POLYPHONIC + 1);
    return data;
}

inline static float chop(float d, Playdata *info, uint64_t t, double speed) {
    uint64_t t0 = info->sampling_rate / speed;
    uint64_t m = t % t0;
    if (m < t0 / 2) return d;
    else return 0;
}

// 同じLPF, HPFを2つ重ねるとバグる(はず)
static float lpf(float d, Playdata *info, uint64_t t, double fc, double Q) {
    double a0, a1, a2, b0, b1, b2;

    fc = tan(PI * (fc / info->sampling_rate)) / (2.0 * PI);
    a0 = 1.0 + 2.0 * PI * fc / Q + 4.0 * PI * PI * fc * fc;
    a1 = (8.0 * PI * PI * fc * fc - 2.0) / a0;
    a2 = (1.0 - 2.0 * PI * fc / Q + 4.0 * PI * PI * fc * fc) / a0;
    b0 = 4.0 * PI * PI * fc * fc / a0;
    b1 = 8.0 * PI * PI * fc * fc / a0;
    b2 = 4.0 * PI * PI * fc * fc / a0;

    static float x1, x2, y1, y2;
    if (t == 0) {
        x2 = 0;
        y2 = 0;
        y1 = d * b0;
        x1 = d;
    } else if (t == 1) {
        x2 = x1;
        y2 = y1;
        y1 = (d * b0) + (x1 * b1) - (y1 * a1);
        x1 = d;
    } else {
        x2 = x1;
        y2 = y1;
        y1 = (d * b0) + (x1 * b1) + (x2 * b2) - (y1 * a1) - (y2 * a2);
        x1 = d;
    }

    return y1;
}

static float hpf(float d, Playdata *info, uint64_t t, double fc, double Q) {
    double a0, a1, a2, b0, b1, b2;

    fc = tan(PI * (fc / info->sampling_rate)) / (2.0 * PI);
    a0 = 1.0 + 2.0 * PI * fc / Q + 4.0 * PI * PI * fc * fc;
    a1 = (8.0 * PI * PI * fc * fc - 2.0) / a0;
    a2 = (1.0 - 2.0 * PI * fc / Q + 4.0 * PI * PI * fc * fc) / a0;
    b0 = 1.0 / a0;
    b1 = -2.0 / a0;
    b2 = 1.0 / a0;

    static float x1, x2, y1, y2;
    if (t == 0) {
        x2 = 0;
        y2 = 0;
        y1 = d * b0;
        x1 = d;
    } else if (t == 1) {
        x2 = x1;
        y2 = y1;
        y1 = (d * b0) + (x1 * b1) - (y1 * a1);
        x1 = d;
    } else {
        x2 = x1;
        y2 = y1;
        y1 = (d * b0) + (x1 * b1) + (x2 * b2) - (y1 * a1) - (y2 * a2);
        x1 = d;
    }

    return y1;
}

static float wah(float d, Playdata *info, uint64_t t, double fc, double Q, double depth, double speed) {
    fc = fc + depth * sin(2 * PI * speed * t / info->sampling_rate);
    return lpf(d, info, t, fc, Q);
}

static float radio(float d, Playdata *info, uint64_t t) {
    return lpf(d, info, t, 1000, 10) * 0.8;
}

float filtering(float data, Playdata *info, uint64_t t) {
    if (info->sound == NULL) {
        return data;
    }
    VectorPTR *filters = info->sound->filters;

    uint64_t i = 0;
    while (i < filters->length) {
        Filter *filter = ((Filter *)(filters->data[i]));
        if (filter == NULL) {
            return data;
        }

        switch (filter->num) {
        case CLIP:
            data = clip(data);
            break;
        case FADE_IN:
            data = fade_in(data, info, t, 
                filter->args[0]->value.f
            );
            break;
        case FADE_OUT:
            data = fade_out(data, info, t,
                filter->args[0]->value.f
            );
            break;
        case FADE:
            data = fade(data, info, t,
                filter->args[0]->value.f,
                filter->args[1]->value.f
            );
            break;
        case AMP:
            data = amp(data, info, t,
                filter->args[0]->value.f
            );
            break;
        case TREMOLO:
            data = tremolo(data, info, t,
                filter->args[0]->value.f,
                filter->args[1]->value.f
            );
            break;
        case DETUNE:
            data = detune(data, info, t,
                filter->args[0]->value.f
            );
            break;
        case CHOP:
            data = chop(data, info, t,
                filter->args[0]->value.f
            );
            break;
        case LPF:
            data = hpf(data, info, t,
                filter->args[0]->value.f,
                M_SQRT1_2
            );
            break;
        case HPF:
            data = lpf(data, info, t,
                filter->args[0]->value.f,
                M_SQRT1_2
            );
            break;
        case WAH:
            data = wah(data, info, t,
                1000,
                filter->args[0]->value.f,
                filter->args[1]->value.f,
                filter->args[2]->value.f
            );
            break;
        case RADIO:
            data = radio(data, info, t);
            break;
        default:
            printf("%I64d\n", filter->num);
            oto_error(OTO_SOUND_PLAYER_ERROR);
        }

        i++;
    }

    return data;
}
