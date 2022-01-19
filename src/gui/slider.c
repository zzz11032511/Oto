#include <oto/oto_gui.h>

void aInitSlider(Slider *s, AInt32a length, AInt32a x, AInt32a y, double value_min, double value_max, AInt32a color) {
    s->length = length;
    s->x = x;
    s->y = y;
    s->value_x = x - 5;

    s->value_min = value_min;
    s->value_max = value_max;
    s->color = color;
}

double aGetValueSlider(Slider *s) {
    return s->value_min + ((double)(s->value_x - s->x + 5) / (s->length - s->x + 10)) * (s->value_max - s->value_min);
}

void aFillSlider(AWindow *w, Slider *s) {
    aFillRect(w, s->length, SLIDER_WEIGHT, s->x, s->y, SLIDER_BASE_COLOR);
    aFillOval(w, SLIDER_CIRCLE_WIDTH, SLIDER_CIRCLE_WIDTH, s->value_x, s->y - 8, s->color);
}

void aKeyIn2Slider(Slider *s, AInt32a key, AInt32a delta) {
    switch (key) {
    case AKEY_LEFT:
        if (s->value_x <= s->x - 5) {
            s->value_x = s->x - 5;
        } else {
            s->value_x -= delta;
        }
        break;
    case AKEY_RIGHT:
        if (s->value_x >= s->length + 5) {
            s->value_x = s->length + 5;
        } else {
            s->value_x += delta;
        }
        break;
    default:
        break;
    }
}
