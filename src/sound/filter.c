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
    {"CRUSH",       5, CRUSH,       1},
    {"LOW_PASS",    8, LOW_PASS,    1}
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

inline static float crush(float d, Playdata *info, uint64_t t, double bits) {
    float q = 0.5 / bits;
    if (q < 0) return d;
    
    float qd = -1.0;
    while (qd <= 1.0) {
        if (qd <= d && d <= (qd + q)) {
            return qd + q / 2;
        }
        qd += q;
    }
    return d / 2;
}

static float low_pass(float d, Playdata *info, uint64_t t, double cutoff, double Q) {
    float fc = tan(PI * cutoff) / (2.0 * PI);
    // float a0 = 1.0 + 2.0 * PI * fc / Q + 4.0 * PI * PI * fc * fc;
    // float a1 = (8.0 * PI * PI * fc * fc - 2.0) / a0;
    // float a2 = (1.0 - 2.0 * PI * fc / Q + 4.0 * PI * PI * fc * fc) / a0;
    // float b0 = 4.0 * PI * PI * fc * fc / a0;
    // float b1 = 8.0 * PI * PI * fc * fc / a0;
    // float b2 = 4.0 * PI * PI * fc * fc / a0;

    float a1 = -0.9428;
    float a2 =  0.3333;
    float b0 =  0.0976;
    float b1 =  0.1953;
    float b2 =  0.0976;

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
        case CRUSH:
            data = crush(data, info, t,
                filter->args[0]->value.f
            );
            break;
        case LOW_PASS:
            data = low_pass(data, info, t,
                filter->args[0]->value.f,
                M_SQRT1_2
            );
            break;
        default:
            printf("%d\n", filter->num);
            oto_error(OTO_SOUND_PLAYER_ERROR);
        }

        i++;
    }

    return data;
}
