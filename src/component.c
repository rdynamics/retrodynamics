#include "component.h"
#include "entity.h"

#include "memory_tracker.h"

#include <stdlib.h>

#define SZ_HEAD (sizeof(c_table))
#define SZ_PARENT (sizeof(entity*))

c_header **c_head(component c) {
	return ((void*)c) - sizeof(c_header*) - SZ_PARENT;
}

component c_body(c_header **head) {
	return ((void*)head) + sizeof(c_header*) + SZ_PARENT;
}

c_table c_tabl(component c) {
	return *c_head(c);
}

entity **c_parent_ptr(component c) {
    return ((void*)c) - SZ_PARENT;
}

component c_init_(c_table table, size_t size) {
	c_table *newc = malloc(SZ_HEAD + SZ_PARENT + size);

    *c_parent_ptr(newc) = NULL;
    
	*newc = table;
	return c_body(newc);
}

void c_free(component c) {
    free(c_head(c));
}

int c_draw(component c) {
    if(c_tabl(c)->draw(c, c_parent(c))) {
        ent_destroy(c_parent(c));
        return 1;
    }
    return 0;
}

int c_tick(component c) {
    if(c_tabl(c)->tick(c, c_parent(c))) {
        ent_destroy(c_parent(c));
        return 1;
    }
    return 0;
}

entity *c_parent(component c) {
    return *c_parent_ptr(c);
}
