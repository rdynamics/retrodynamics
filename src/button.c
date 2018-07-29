#include "button.h"

float button_value(button *b) {
    if(b->ticks > 3) return 1;
    switch(b->ticks) {
        case 0: return 0;
        case 1: return 0.25f;
        case 2: return 0.5f;
        case 3: return 0.75f;
    }
    return 0;
}

int on_down(button *b) {
    return b->is_down && !b->was_down;
}

int on_up(button *b) {
    return !b->is_down && b->was_down;
}

axis key_axis(int negative, int positive) {
    return (axis){ get_key(negative), get_key(positive) };
}

float axis_value(axis a) {
    return button_value(a.positive) - button_value(a.negative);
}

joystick joystick_from(axis h, axis v) {
    return (joystick){ h, v };
}

/* Transforms coordinates from a "square" joystick (i.e. a keyboard)
 * into a circular joystick
 */
vec joystick_value(joystick j) {
    float x = axis_value(j.horizontal);
    float y = axis_value(j.vertical);
    
    vec v = vxy(x, y);
    
    if(!x && !y) return v0();
    
    /* The amount of length that a vector could have in its direction */
    float potential_length2 = 1;
    if(x) potential_length2 += (y * y) / (x * x);

    if(potential_length2 > 2) {
        potential_length2 = 1 + (x * x) / (y * y);
    }
    
    float length = sqrt(len2(v) / potential_length2);
    
    v = vmul(normalize(v), length);
    return v;
}