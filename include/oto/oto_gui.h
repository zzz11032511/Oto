#pragma once

#include <acl.h>

#define PRINTWAV_WIN_WIDTH  1000
#define PRINTWAV_WIN_HEIGHT 200
#define PRINTWAV_WIN_BACKGROUND_COLOR 0x2e2e2e

#define PRINTWAV_OVERALL_WAVE_COLOR 0x00c3ff
#define PRINTWAV_ZOOM_WAVE_COLOR    0x00c3ff

#define SYNTH_WIN_WIDTH  600
#define SYNTH_WIN_HEIGHT 350
#define SYNTH_WIN_BACKGROUND_COLOR 0xededed

typedef struct {
    AInt32a length;
    AInt32a x;
    AInt32a y;
    AInt32a color;

    AInt32a value_x;
    double value_min;
    double value_max;
} Slider;

#define SLIDER_BASE_COLOR 0xa0a0a0
#define SLIDER_WEIGHT 4
#define SLIDER_CIRCLE_WIDTH 20

void aInitSlider(Slider *s, AInt32a length, AInt32a x, AInt32a y, double value_min, double value_max, AInt32a color);

void aFillSlider(AWindow *w, Slider *s);

void aKeyIn2Slider(Slider *s, AInt32a key, AInt32a delta);
double aGetValueSlider(Slider *s);