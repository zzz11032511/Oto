#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <acl.h>

#include "../../util/util.h"
#include "../../error/error.h"
#include "../../variable/type.h"
#include "../../variable/var_stack.h"
#include "../../variable/variable.h"
#include "../../sound/sound.h"
#include "../../sound/track.h"
#include "../../sound/filter/filter.h"
#include "../../sound/sound_io.h"
#include "../../sound/oscillator.h"

void define_sound(var_t *sound, var_t *wave) {
    sound->type = TySound;
    sound->value.pVal = (void *)new_sound((uint64_t)wave->value.fVal, NULL, 0);
}

void copy_sound(var_t *new, var_t *sound) {
    wave_t wave = ((SOUND)sound->value.pVal)->wave;
    if (new->type != TySound) {
        new->type = TySound;
        new->value.pVal = (void *)new_sound(wave, NULL, 0);
    } else {
        ((SOUND)new->value.pVal)->wave = wave;
    }

    memcpy(
        ((SOUND)new->value.pVal)->filters, 
        ((SOUND)sound->value.pVal)->filters,
        sizeof(var_t) * MAX_CONNECT
    );

    ((SOUND)new->value.pVal)->num_of_filter = 
        ((SOUND)sound->value.pVal)->num_of_filter;
}

void connect_filter(struct var_stack *stack, var_t *sound, var_t *filter) {
    if (sound->type != TySound || filter->type != TyFilter) {
        call_error(UNKNOWN_ERROR, "connect_filter()");
    }

    struct var_stack sub_stack;
    sub_stack.sp = 0;

    int32_t num_of_params = ((FILTER)filter->value.pVal)->param;
    
    // 現在つながっているフィルター
    var_t *connected = ((SOUND)sound->value.pVal)->filters; 
    // 現在つながっているフィルターの数
    int32_t ptr = ((SOUND)sound->value.pVal)->num_of_filter;

    connected[ptr++] = *filter;

    // 引数の順番が逆なので最初に一時的なスタックに移してから接続する
    for (int32_t i = 0; i < num_of_params; i++) {
        vpush(&sub_stack, vpop(stack));
    }

    for (int32_t i = 0; i < num_of_params; i++) {
        connected[ptr++] = vpop(&sub_stack);
    }

    ((SOUND)sound->value.pVal)->num_of_filter = ptr;
}

void beep(struct var_stack *stack) {
    var_t t1 = vpop(stack);  // length
    var_t t2 = vpop(stack);  // frequency

    if (t1.type == TyInitVal) {
        t1.value.fVal = 1;
    }

    if (t2.type == TyInitVal) {
        t2.value.fVal = 500;
    }

    printf("[Beep] ");
    printf("frequency : %8.3f[Hz], length : %2.2f[s]\n", 
           t2.value.fVal, t1.value.fVal);

    Beep((uint64_t)t2.value.fVal, (uint64_t)(t1.value.fVal * 1000));
}

void play(struct var_stack *stack, uint64_t samples_per_sec) {
    var_t t1 = vpop(stack);  // sound
    var_t t2 = vpop(stack);  // velocity
    var_t t3 = vpop(stack);  // length
    var_t t4 = vpop(stack);  // frequency

    uint64_t velocity = 100;
    if (t2.type != TyInitVal) {
        velocity = (uint64_t)t2.value.fVal;
        if (velocity > 100) {
            velocity = 100;
        }
    }

    uint64_t length = samples_per_sec;  // 1秒
    if (t3.type != TyInitVal) {
        length = (uint64_t)(t3.value.fVal * samples_per_sec);
    }

    // if (length < 1600) {
    //     // lengthが1600未満の時に何故かエラーが発生する
    //     call_error(SOUND_PLAYER_ERROR);
    // }

    double freq = 500;
    if (t4.type != TyInitVal) {
        freq = t4.value.fVal;
    }

    SOUND s = NULL;
    wave_t wave = SINE_WAVE;
    if (t1.type == TySound) {
        s = (SOUND)t1.value.pVal;
        wave = s->wave;
    }

    TRACK t = new_track(length, MONO, samples_per_sec, 16, velocity);
    if (velocity != 0) {
        write_wave(wave, t, freq, length, samples_per_sec, 0, 1);
    }

    if (t1.type == TySound) {
        filtering(t, s);
    }

    printf("[Play] ");
    printf("frequency : %8.3f, length : %2.2f, velocity : %3I64d, wave : %3d\n", 
           freq, t3.value.fVal, velocity, wave);

    play_track(t);
}

static AInt16a transform_tdata(double data, AInt32a height) {
    if (data > 1) {
        data = 1;
    } else if (data < -1) {
        data = -1;
    }
    return (AInt16a)((height / 2) - (data * (height / 2) * 0.8));
}

static void print_wave_sub(TRACK t) {
    AInt16a width  = 1000;
    AInt16a height = 200;
    AWindow *w = aOpenWin(width, height * 2, "wave", 1);

    AInt32a color = aRgb8(0, 0, 0);
    for (AInt16a y = 0; y < height; y++) {
        for (AInt16a x = 0; x < width; x++) {
            aSetPix0(w, x, y, color);
        }
    }

    // 全体の波形
    color = aRgb8(0, 255, 0);
    int32_t a = t->length / width;
    for (int32_t i = 0; i < width; i++) {
        AInt16a y = transform_tdata(t->data[i * a], height);
        aSetPix0(w, i, y, color);
    }

    // 拡大した波形
    color = aRgb8(255, 0, 0);
    for (int32_t i = 0; i < width; i++) {
        AInt16a y = transform_tdata(t->data[i + t->length / 2], height) + height;
        aSetPix0(w, i, y, color);
    }

    aWait(-1);
}

void print_wave(struct var_stack *stack, uint64_t samples_per_sec) {
    var_t t1 = vpop(stack);  // sound
    var_t t2 = vpop(stack);  // velocity
    var_t t3 = vpop(stack);  // length
    var_t t4 = vpop(stack);  // frequency

    uint64_t velocity = 100;
    if (t2.type != TyInitVal) {
        velocity = (uint64_t)t2.value.fVal;
        if (velocity > 100) {
            velocity = 100;
        }
    }

    uint64_t length = samples_per_sec;  // 1秒
    if (t3.type != TyInitVal) {
        length = (uint64_t)(t3.value.fVal * samples_per_sec);
    }

    double freq = 500;
    if (t4.type != TyInitVal) {
        freq = t4.value.fVal;
    }

    SOUND s = NULL;
    wave_t wave = SINE_WAVE;
    if (t1.type == TySound) {
        s = (SOUND)t1.value.pVal;
        wave = s->wave;
    }

    TRACK t = new_track(length, MONO, samples_per_sec, 16, velocity);
    if (velocity != 0) {
        write_wave(wave, t, freq, length, samples_per_sec, 0, 1);
    }

    if (t1.type == TySound) {
        filtering(t, s);
    }

    printf("[Play] ");
    printf("frequency : %8.3f, length : %2.2f, velocity : %3I64d, wave : %3d\n", 
           freq, t3.value.fVal, velocity, wave);

    play_track(t);

    print_wave_sub(t);
}