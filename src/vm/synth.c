#include <oto/oto.h>
#include <oto/oto_gui.h>
#include <oto/oto_sound.h>

#define SYNTH_NUM 4

#define SLIDER_LENGTH 550
#define SLIDER_X 25

#define CURRENT_SLIDER_COLOR 0xff2414
#define SUB_SLIDER_COLOR 0x40bfff

typedef struct {
    Slider slider;
    Var *var;
    bool use_flag;
} SynthObject;

SynthObject synths[SYNTH_NUM];
Sound *sound;
double synth_freq;
double loop_point = 10;

void init_synth() {
    AInt32a color = 0;
    for (int64_t i = 0; i < SYNTH_NUM; i++) {
        synths[i].use_flag = false;
        synths[i].var = NULL;
        if (i == 0) {
            color = CURRENT_SLIDER_COLOR;
        } else {
            color = SUB_SLIDER_COLOR;
        }

        aInitSlider(
            &synths[i].slider,
            SLIDER_LENGTH,
            SLIDER_X,
            50 + i * 75,
            0,
            0,
            color
        );
    }
}

void set_synth(int64_t no, Sound *s, Var *v, double min, double max, double freq) {
    if (!(1 <= no && no <= SYNTH_NUM)) {
        oto_error(OTO_UNKNOWN_ERROR);
    }
    no = no - 1;

    sound = s;
    synths[no].var = v;
    synths[no].use_flag = true;
    synths[no].slider.value_min = min;
    synths[no].slider.value_max = max;
    synth_freq = freq;
}

void start_synth(Status *status) {
    int64_t use_num = 0;
    for (int64_t i = 0; i < SYNTH_NUM; i++) {
        if (synths[i].use_flag) use_num++;
    }
    if (use_num == 0) return;

    AWindow *w = aOpenWin(SYNTH_WIN_WIDTH, SYNTH_WIN_HEIGHT, "SYNTH", 1);

    Playdata data;
    data.freq[0] = synth_freq;
    data.sampling_rate = status->sampling_rate;
    data.sound = sound;
    data.sound_num = 1;
    data.volume = 50;
    data.length = loop_point * status->sampling_rate;

    write_out_data(data, false, false);
    set_stream_active_flag(true);
    
    AInt32a key = 0;
    AInt32a select = 1;
    for (;;) {
        aFillRect0(w, SYNTH_WIN_WIDTH, SYNTH_WIN_HEIGHT, 0, 0, SYNTH_WIN_BACKGROUND_COLOR);
        for (int64_t i = 0; i < SYNTH_NUM; i++) {
            if (synths[i].use_flag) {
                char str[64];
                sprintf(str, "VARNAME : %20s  VALUE : %.3f", synths[i].var->token->str, aGetValueSlider(&synths[i].slider));
                aDrawStr0(w, synths[i].slider.x, synths[i].slider.y + 20, 0, SYNTH_WIN_BACKGROUND_COLOR, str);
                aFillSlider(w, &synths[i].slider);
                synths[i].var->value.f = aGetValueSlider(&synths[i].slider);
            }
        }

        char str[64];
        sprintf(str, "select : %d", select);
        aDrawStr0(w, 5, 5, 0, SYNTH_WIN_BACKGROUND_COLOR, str);
        aDrawStr0(w, 5, SYNTH_WIN_HEIGHT - 16 - 5, 0, SYNTH_WIN_BACKGROUND_COLOR, "Enter : exit   1, 2, 3, 4 : synth change");
        
        key = aInkey(w, 3);
        switch (key) {
        case AKEY_ENTER:
            goto end_proc;
        case AKEY_1:
            synths[select - 1].slider.color = SUB_SLIDER_COLOR;
            select = 1;
            synths[select - 1].slider.color = CURRENT_SLIDER_COLOR;
            break;
        case AKEY_2:
            synths[select - 1].slider.color = SUB_SLIDER_COLOR;
            select = 2;
            synths[select - 1].slider.color = CURRENT_SLIDER_COLOR;
            break;
        case AKEY_3:
            synths[select - 1].slider.color = SUB_SLIDER_COLOR;
            select = 3;
            synths[select - 1].slider.color = CURRENT_SLIDER_COLOR;
            break;
        case AKEY_4:
            synths[select - 1].slider.color = SUB_SLIDER_COLOR;
            select = 4;
            synths[select - 1].slider.color = CURRENT_SLIDER_COLOR;
            break;
        case AKEY_LEFT:
        case AKEY_RIGHT:
            aKeyIn2Slider(&synths[select - 1].slider, key, 5);
        default:
            break;
        }

        if (is_stream_active()) {
            aWait(1);
        } else {
            // loop_point秒流したらもう一回最初から流す
            // クリップノイズが発生するのをどうにかしたい
            write_out_data(data, false, false);
            set_stream_active_flag(true);            
        }
    }

end_proc:
    return;
}
