#ifndef RD_EVENT_H
#define RD_EVENT_H

#define RD_INTERNAL

#include <glfw/GLFW3.h>

#include "button.h"

void events_save();
void events_clear();

void events_attach(GLFWwindow*);

#endif