#ifndef RD_BUTTON_H
#define RD_BUTTON_H

#include "vector.h"

vec screen_cursor();
vec world_cursor();

#define LEFT_MOUSE 497
#define MIDDLE_MOUSE 498
#define RIGHT_MOUSE 499

typedef struct {
    int is_down;
    int was_down;
    int ticks;
} button;

button *get_key(int);

int on_down(button*);
int on_up(button*);

vec get_scroll();

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