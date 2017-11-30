#ifndef RD_GRAPHICS_H
#define RD_GRAPHICS_H

#include "component.h"
#include "vector.h"

typedef struct {
    float r, g, b;
} color;

typedef struct {
    float x0, y0, x1, y1;
    float width, height;
} sprite;

declare_body(sprite_renderer, {
    sprite *spr;
    vec center;
})

void set_background(color);
void draw_sprite(const sprite*,float,float);

#endif