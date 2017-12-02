#ifndef RD_BUTTON_H
#define RD_BUTTON_H

#include "vector.h"

typedef struct {
    int is_down;
    int was_down;
    int ticks;
} button;

button *get_key(int);

float button_value(button*);

typedef struct {
    button *negative;
    button *positive;
} axis;

axis key_axis(int,int);
float axis_value(axis);

typedef struct {
    axis horizontal;
    axis vertical;
} joystick;

joystick joystick_from(axis,axis);
vec joystick_value(joystick);

#endif