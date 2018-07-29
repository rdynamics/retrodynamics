#include "event.h"

static button keyboard[500];

static int new_key_states[500];

button *get_key(int which) {
    if(which < 0 || which >= 500) return NULL;
    return &keyboard[which];
}

void key_callback(GLFWwindow* w, int key, int scancode, int action, int mods) {
    if(key >= 0 && key < 498) {
        if(action == GLFW_PRESS) {
            new_key_states[key] = 2;
        }
        else if(action == GLFW_RELEASE) {
            new_key_states[key] = 1;
        }
    }
}

vec absolute_cursor;

void cursor_position_callback(GLFWwindow *w, double x, double y) {
    absolute_cursor = vxy(x, y);
}

void mouse_button_callback(GLFWwindow *w, int button, int action, int mods) {
    if(action == GLFW_PRESS) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                new_key_states[LEFT_MOUSE] = 2;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                new_key_states[MIDDLE_MOUSE] = 2;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                new_key_states[RIGHT_MOUSE] = 2;
                break;
        }
    }
    if(action == GLFW_RELEASE) {
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                new_key_states[LEFT_MOUSE] = 1;
                break;
            case GLFW_MOUSE_BUTTON_MIDDLE:
                new_key_states[MIDDLE_MOUSE] = 1;
                break;
            case GLFW_MOUSE_BUTTON_RIGHT:
                new_key_states[RIGHT_MOUSE] = 1;
                break;
        }
    }
}

static vec scroll;

vec get_scroll() {
	return scroll;
}

void scroll_callback(GLFWwindow *window, double x, double y) {
	scroll = vxy(x, y);
}

void events_attach(GLFWwindow* w) {
    glfwSetKeyCallback(w, key_callback);
    glfwSetCursorPosCallback(w, cursor_position_callback);
    glfwSetMouseButtonCallback(w, mouse_button_callback);
	glfwSetScrollCallback(w, scroll_callback);
    
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
        if(keyboard[i].is_down) {
            ++keyboard[i].ticks;
        } else { keyboard[i].ticks = 0; }
    }
}

void events_clear() {
    for(size_t i = 0; i < 500; ++i) {
        keyboard[i].was_down = keyboard[i].is_down;
    }
	scroll = v0();
}