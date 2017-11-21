#include "window.h"

static int view_width, view_height;

static void resize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    int viewport_width, viewport_height;
    if((float)height / width > (float)view_height / view_width) {
        viewport_width = width;
        viewport_height = ((float)view_height / view_width) * width;
    }
    else {
        viewport_width = ((float)view_width / view_height) * height;
        viewport_height = height;
    }
    
    int offset_x = (width - viewport_width) / 2;
    int offset_y = (height - viewport_height) / 2;
   
    glOrtho(0, view_width, 0, view_height, -60, 60);
    glViewport(offset_x, offset_y, viewport_width, viewport_height);
}

static void resize_callback(GLFWwindow* w, int width, int height) {
    resize(width, height);
}

GLFWwindow *window_init(window_info *info) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;
    
    view_width = info->width;
    view_height = info->height;
    
    int initial_width = view_width * info->scale;
    int initial_height = view_height * info->scale;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(initial_width, initial_height, info->title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }

    glfwSetFramebufferSizeCallback(window, resize_callback);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    resize(initial_width, initial_height);
    
    return window;
}