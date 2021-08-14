#pragma once

#include "../utils/util.h"

// 和音を単音同士のOR演算で実現しようとしている
// オクターブをまたいだ和音はどうするのか未定
#define C       0x00000001
#define C_SHARP 0x00000002
#define D       0x00000004
#define D_SHARP 0x00000008
#define E       0x00000010
#define F       0x00000020
#define F_SHARP 0x00000040
#define G       0x00000080
#define G_SHARP 0x00000100
#define A       0x00000200
#define A_SHARP 0x00000400
#define B       0x00000800

struct note {
    uint64_t note;
    int32_t octave;
    double note_frequency;
};

const struct note note_list[] = {
    {C,       4, 269.292},
    {C_SHARP, 4, 285.305},
    {D,       4, 302.270},
    {D_SHARP, 4, 320.244},
    {E,       4, 339.286},
    {F,       4, 359.461},
    {F_SHARP, 4, 380.836},
    {G,       4, 403.482},
    {G_SHARP, 4, 427.474},
    {A,       4, 452.893},
    {A_SHARP, 4, 479.823},
    {B,       4, 508.355},
}
