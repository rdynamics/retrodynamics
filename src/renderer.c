#include "graphics.h"

#include <GLFW/glfw3.h>
#include <stdio.h>

#include "images_diffuse.h"

color background;

static int camera_width, camera_height;

static GLuint spritesheet;

void load_textures() {
    spritesheet = load_image_diffuse();
}

void set_camera_size(int width, int height) {
    camera_width = width;
    camera_height = height;
}

void set_background(color bg) {
    background = bg;
}

void render() {
    glBindTexture(GL_TEXTURE_2D, 0);
    glColor3f(background.r, background.g, background.b);
    
    glBegin(GL_QUADS);
        glVertex2f(0.f, 0.f);
        glVertex2f(0.f, camera_height);
        glVertex2f(camera_width, camera_height);
        glVertex2f(camera_width, 0);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, spritesheet);
    glColor3f(1.f, 1.f, 1.f);
    
    draw_sprite(&sprite_brick, 0, 0);
}

void draw_sprite(const sprite *s, float x, float y) {    
    glBegin(GL_QUADS);
        glTexCoord2f(s->x0, s->y0);
        glVertex2f(x, y);
        
        y += s->height;
        
        glTexCoord2f(s->x0, s->y1);
        glVertex2f(x, y);
        
        x += s->width;
        
        glTexCoord2f(s->x1, s->y1);
        glVertex2f(x, y);
        
        y -= s->height;
        
        glTexCoord2f(s->x1, s->y0);
        glVertex2f(x, y);
    glEnd();
}