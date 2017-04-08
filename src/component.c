#include "component.h"

#include <stdlib.h>

#define SZ_HEAD (sizeof(c_table))

c_header **c_head(component c) {
	return ((void*)c) - sizeof(c_header*);
}

component c_body(c_header **head) {
	return ((void*)head) + sizeof(c_header*);
}

c_table c_tabl(component c) {
	return *c_head(c);
}

component c_init_(c_table table, size_t size) {
	c_table *newc = malloc(SZ_HEAD + size);

	*newc = table;
	return c_body(newc);
}

void c_free(component c) {
    free(c_head(c));
}

void c_update(component c) {
    c_tabl(c)->update(c);
}

void c_tick(component c) {
    c_tabl(c)->tick(c);
}