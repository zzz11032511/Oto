#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <windows.h>

#include "../../error/error.h"
#include "../../variable/type.h"
#include "../../variable/var_stack.h"
#include "../../variable/variable.h"
#include "../../sound/sound.h"
#include "../../sound/track.h"
#include "../../sound/sound_io.h"
#include "../../sound/oscillator.h"

void define_sound(var_t *v1, var_t *v2) {
    v1->type = TySound;
    v1->value.pVal = (void *)new_sound((uint64_t)v2->value.fVal, NULL, 0);
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
           t2.value.fVal, t1.value.fVal * 1000);

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

    printf("[Play] ");
    printf("frequency : %8.3f, length : %2.2f, velocity : %3I64d, wave : %3d\n", 
           freq, t3.value.fVal, velocity, wave);

    play_track(t);
}