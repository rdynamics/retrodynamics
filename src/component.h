#ifndef RD_COMPONENT_H
#define RD_COMPONENT_H

#include <stdlib.h>

/*
 * Layout of a component:
 * - Pointer to header (c_table)
 * - Pointer to c_body
 */
 
struct entity;

typedef int (*c_method)(void*,struct entity*);

typedef struct c_header {
    c_method draw;
    c_method tick;
} c_header;

typedef c_header *c_table;
typedef void *component;

extern c_header **c_head(component);
extern component  c_body(c_header**);
extern c_table    c_tabl(component);

extern component c_init_(c_table,size_t);
extern void c_free(component);

extern int c_draw(component);
extern int c_tick(component);

extern struct entity *c_parent(component);
extern struct entity **c_parent_ptr(component);

#define c_init(table, body) (c_init_(&table, sizeof(body)))
#define c_h(type) h_ ## type
#define c_b(type) struct b_ ## type
#define c_to(comp, type)  (c_b(type)*)(comp)
#define c_new(type) c_init(c_h(type), c_b(type))

#define c_type(type) (&c_h(type))
#define c_isa(comp, type) (c_tabl(comp) == c_type(type))

#define declare_body(name, body) typedef c_b(name) body name; extern c_header c_h(name);
#define declare_draw(name, ...)\
    int name ## _draw(void *accessor, entity *parent) {\
        name *self = accessor;\
        { __VA_ARGS__ }\
        return 0;\
    }
#define declare_tick(name, ...)\
    int name ## _tick(void *accessor, entity *parent) {\
        name *self = accessor;\
        { __VA_ARGS__ }\
        return 0;\
    }
#define declare_head(name)\
    c_header c_h(name) = {\
        .draw = &name ## _draw,\
        .tick   = &name ## _tick\
    };

#endif
