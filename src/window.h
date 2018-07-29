#ifndef RD_WINDOW_H
#define RD_WINDOW_H

#include <glfw/GLFW3.h>

typedef struct {
    const char *title;
    int width, height;
    float scale;
} window_info;

GLFWwindow *window_init(window_info*);

int get_screen_width();
int get_screen_height();

#endif