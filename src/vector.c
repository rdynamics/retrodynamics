#include "vector.h"
#include <math.h>

vec vxy(float x, float y) { return (vec){ x, y }; }
vec vx(float x) { return vxy(x, 0); }
vec vy(float y) { return vxy(0, y); }
vec v0(void) { return vxy(0, 0); }

vec vadd(vec a, vec b) { return vxy(a.x + b.x, a.y + b.y); }
vec vsub(vec a, vec b) { return vxy(a.x - b.x, a.y - b.y); }

vec vmul(vec v, float s) { return vxy(v.x * s, v.y * s); }
vec vdiv(vec v, float s) { return vxy(v.x / s, v.y / s); }

float dot(vec a, vec b) { return a.x * b.x + a.y * b.y; }
float cross(vec a, vec b) { return a.x * b.y - a.y * b.x; }

float vcos(vec a, vec b) {
	return dot(a, b) / (len(a) * len(b));
}

float len(vec v) { return sqrt(len2(v)); }
float len2(vec v) { return v.x * v.x + v.y * v.y; }

vec normalize(vec v) {
    if(v.x || v.y) return vdiv(v, len(v));
    return v0();
}

vec vsnap(vec v, float step) {
    return vxy(round(v.x / step) * step, round(v.y / step) * step);
}