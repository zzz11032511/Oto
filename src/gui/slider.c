#include <acl.h>

#define WIN_WIDTH  600
#define WIN_HEIGHT 400

#define WIN_BACKGROUND_COLOR 0xededed

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

void aInitSlider(Slider *s, AInt32a length, AInt32a x, AInt32a y, double value_min, double value_max, AInt32a color) {
    s->length = length;
    s->x = x;
    s->y = y;
    s->value_x = x - 5;

    s->value_min = value_min;
    s->value_max = value_max;
    s->color = color;
}

void aFillSlider(AWindow *w, Slider *s) {
    aFillRect(w, s->length, SLIDER_WEIGHT, s->x, s->y, SLIDER_BASE_COLOR);
    aFillOval(w, SLIDER_CIRCLE_WIDTH, SLIDER_CIRCLE_WIDTH, s->value_x, s->y - 8, s->color);
}

void aKeyIn2Slider(Slider *s, AInt32a key, AInt32a delta) {
    printf("%x\n", key);
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

inline double aGetValueSlider(Slider *s) {
    return s->value_min + ((double)(s->value_x - s->x + 5) / (s->length - s->x + 10)) * s->value_max;
}

int main(void) {
    AWindow *w = aOpenWin(WIN_WIDTH, WIN_HEIGHT, "test", 1);
    
    Slider s;
    aInitSlider(&s, 550, 25, 50, 0, 100, 0x40bfff);

    AInt32a key = 0;
    for (;;) {
        aFillRect0(w, WIN_WIDTH, WIN_HEIGHT, 0, 0, WIN_BACKGROUND_COLOR);
        aFillSlider(w, &s);
        
        key = aInkey(w, 1);
        switch (key) {
        case AKEY_ENTER:
            goto end_proc;
        case AKEY_LEFT:
        case AKEY_RIGHT:
            aKeyIn2Slider(&s, key, 5);
            break;
        default:
            break;
        }

        printf("value : %f\n", aGetValueSlider(&s));
        aWait(1);
    }

end_proc:
    printf("success\n");

    return 0;
}