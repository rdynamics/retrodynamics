#ifndef RD_GRAPHICS_H
#define RD_GRAPHICS_H

typedef struct {
    float r, g, b;
} color;

typedef struct {
    float x0, y0, x1, y1;
    float width, height;
} sprite;

void set_background(color);
void draw_sprite(const sprite*,float,float);

#endif