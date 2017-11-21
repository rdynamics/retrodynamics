#ifndef RD_BUTTON_H
#define RD_BUTTON_H

typedef struct {
    int is_down;
    int was_down;
} button;

button *get_key(int);

#endif