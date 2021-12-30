#include <oto/oto.h>
#include <oto/oto_sound.h>

const struct init_define_filters def_filters[] = {
    {"CLIP",        4, CLIP,        0},
    {"FADE_IN",     7, FADE_IN,     1},
    {"FADE_OUT",    8, FADE_OUT,    1},
    {"FADE",        4, FADE,        2},
    {"AMP",         3, AMP,         1},
    {"TREMOLO",     7, TREMOLO,     2},
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
        default:
            printf("%d\n", filter->num);
            oto_error(OTO_SOUND_PLAYER_ERROR);
        }

        i++;
    }

    return data;
}
