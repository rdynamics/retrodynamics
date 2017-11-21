#include "event.h"

static button keyboard[500];

static int new_key_states[500];

button *get_key(int which) {
    if(which < 0 || which >= 500) return NULL;
    return &keyboard[which];
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
    if(key >= 0 && key < 500) {
        if(action == GLFW_PRESS) {
            new_key_states[key] = 2;
        }
        else if(action == GLFW_RELEASE) {
            new_key_states[key] = 1;
        }
    }
}

void events_attach(GLFWwindow* w) {
    glfwSetKeyCallback(w, key_callback);
    
    for(size_t i = 0; i < 500; ++i) {
        keyboard[i].is_down = keyboard[i].was_down = new_key_states[i] = 0;
    }
}

void events_save() {
    for(size_t i = 0; i < 500; ++i) {
        if(new_key_states[i]) {
            keyboard[i].is_down = new_key_states[i] - 1;
        }
        new_key_states[i] = 0;
    }
}

void events_clear() {
    for(size_t i = 0; i < 500; ++i) {
        keyboard[i].was_down = keyboard[i].is_down;
    }
}