#ifndef RD_COMPONENT_H
#define RD_COMPONENT_H

#include <stdlib.h>

/*
 * Layout of a component:
 * - Pointer to header (c_table)
 * - Pointer to c_body
 */

typedef void (*c_method)(void*);

typedef struct c_header {
    c_method update;
    c_method tick;
} c_header;

typedef c_header *c_table;
typedef void *component;

extern c_header **c_head(component);
extern component  c_body(c_header**);
extern c_table    c_tabl(component);

extern component c_init_(c_table,size_t);
extern void c_free(component);

extern void c_update(component);
extern void c_tick(component);

#define c_init(table, body) (c_init_(&table, sizeof(body)))
#define c_h(type) h_ ## type
#define c_b(type) struct b_ ## type
#define c_to(comp, type)  (c_b(type)*)(comp)
#define c_new(type) c_init(c_h(type), c_b(type))

#define c_type(type) (&c_h(type))
#define c_isa(comp, type) (c_tabl(comp) == c_type(type))

#define declare_body(name, body) typedef c_b(name) body name
#define declare_update(name, body)\
    void name ## _update(void *accessor) {\
        name *self = accessor;\
        body\
    }
#define declare_tick(name, body)\
    void name ## _tick(void *accessor) {\
        name *self = accessor;\
        body\
    }
#define declare_head(name)\
    c_header c_h(name) = {\
        .update = &name ## _update,\
        .tick   = &name ## _tick\
    }

#endif
