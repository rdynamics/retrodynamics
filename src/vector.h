#ifndef RD_VECTOR_H
#define RD_VECTOR_H

typedef struct {
    float x, y;
} vec;

extern vec v0(void);
extern vec vxy(float,float);
extern vec vx(float);
extern vec vy(float);

extern vec vadd(vec,vec);
extern vec vsub(vec,vec);

extern vec vmul(vec,float);
extern vec vdiv(vec,float);

extern float dot(vec,vec);
extern float cross(vec,vec);

extern float vcos(vec, vec);

extern float len(vec);
extern float len2(vec);

extern vec normalize(vec);

extern vec vsnap(vec,float);

#endif