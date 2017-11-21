#ifndef RD_ENTITY_H
#define RD_ENTITY_H

#include <zgcl/zlist.h>

#include "component.h"
#include "vector.h"

typedef struct {
    vec position;
    zlist_of(component) components;
} entity;

extern entity *ent_new(void);
extern void ent_destroy(entity*);

#endif