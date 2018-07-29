#include "window.h"
#include "event.h"

static int view_width, view_height;
static int viewport_width, viewport_height;
static int offset_x, offset_y;

static int screen_width, screen_height;

int get_screen_width() { return screen_width; }
int get_screen_height() { return screen_height; }

static void resize(int width, int height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    if((float)height / width > (float)view_height / view_width) {
        viewport_width = width;
        viewport_height = ((float)view_height / view_width) * width;
    }
    else {
        viewport_width = ((float)view_width / view_height) * height;
        viewport_height = height;
    }
    
    offset_x = (width - viewport_width) / 2;
    offset_y = (height - viewport_height) / 2;
   
    glOrtho(0, view_width, 0, view_height, -60, 60);
    glViewport(offset_x, offset_y, viewport_width, viewport_height);
	
	screen_width = width;
	screen_height = height;
}

static void resize_callback(GLFWwindow* w, int width, int height) {
    resize(width, height);
}

vec screen_cursor() {
    vec result;
    result.x = ((absolute_cursor.x - offset_x) * view_width) / (float)viewport_width;
    result.y = view_height - (((absolute_cursor.y - offset_y) * view_height) / (float)viewport_height);
    return result;
}

void set_icon(GLFWwindow*);

GLFWwindow *window_init(window_info *info) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return NULL;
    
    view_width = info->width;
    view_height = info->height;
    
    int initial_width = view_width * info->scale;
    int initial_height = view_height * info->scale;
	
	screen_width = initial_width;
	screen_height = initial_height;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(initial_width, initial_height, info->title, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return NULL;
    }
	
	set_icon(window);

    glfwSetFramebufferSizeCallback(window, resize_callback);
	
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    resize(initial_width, initial_height);
    
    return window;
}