#include "entity.h"
#include "new.h"

#include <stdarg.h>

zlist_of(entity*) entity_pool;

size_t minimum_empty = 0;
size_t min = 0;
size_t max_plus_one = 0;

void ent_init(void) {
    /* Start the entity pool with 100 entities */
    zlist_init(entity_pool, 100);
    for(size_t i = 0; i < zsize(entity_pool); ++i) {
        entity_pool[i] = NULL;
    }
}

void ent_cleanup(void) {
    for(size_t i = 0; i < zsize(entity_pool); ++i) { 
        if(entity_pool[i]) {
            zfree(entity_pool[i]->components);
            free(entity_pool[i]);
        }
    }
}

entity *ent_new(void) {
    entity *ent = snew(entity);
    
    ent->position = v0();
    ent->tag = UNTAGGED;
    zlist_init(ent->components, 0);
    
    for(size_t i = minimum_empty; i < zsize(entity_pool); ++i) {
        if(!entity_pool[i]) {
            entity_pool[i] = ent;
            minimum_empty = i + 1;
            if(i < min) min = i;
            if(i + 1 > max_plus_one) max_plus_one = i + 1;
            return ent;
        }
    }
    max_plus_one = zsize(entity_pool) + 1;
    minimum_empty = zsize(entity_pool) + 1;
    zlist_add(entity_pool, ent);
    return ent;
}

void ent_destroy(entity *e) {
    if(!e) return;
    
    int is_min = 1;
    
    for(size_t i = 0; i < zsize(entity_pool); ++i) {
        if(entity_pool[i] == e) {
            if(i < minimum_empty) minimum_empty = i;
            entity_pool[i] = NULL;
            /* the new minimum will be the entity immediately after this one, because
             * now this one will be null
             */
            if(is_min) min = i + 1;
            /* the new maximum can simply be stepped back, because now this one is null
             */
            if(i + 1 == max_plus_one) --max_plus_one;
            break;
        } else if(entity_pool[i]) is_min = 0;
    }
    
    zfree(e->components);
    free(e);
}

entity *ent_all_next(void *data) {
    size_t i = *(size_t*)(data);
    if(i >= max_plus_one) return NULL;
    
    size_t count = *(size_t*)(data + sizeof(size_t));
    ent_condition *list = (ent_condition*)(data + 2 * sizeof(size_t));
    
    //ent_condition c = *(ent_condition*)(data + sizeof(size_t));
    
    size_t current = i;
    ++i;
    for(;;) {
        if(i >= max_plus_one) break;
        if(entity_pool[i]) {
            for(size_t j = 0; j < count; ++j) {
                if(!list[j].compatible(entity_pool[i], list[j].data)) {
                    goto incompatible;
                }
            }
            break;
            incompatible: { }
        }
        ++i;
    }
    *(size_t*)(data) = i;
    return entity_pool[current];
}

void ent_all_cleanup(void *data) {
    size_t count = *(size_t*)(data + sizeof(size_t));
    ent_condition *list = (ent_condition*)(data + 2 * sizeof(size_t));
    for(size_t i = 0; i < count; ++i) {
        list[i].cleanup(list[i].data);
    }
    free(data);
}

int ent_all_has_next(void *data) {
    size_t i = *(size_t*)(data);
    return i != zsize(entity_pool);
}

ent_iterator ent_all_(size_t len, ...) {
    va_list args;
    va_start(args, len);
    
    void *data = malloc(2 * sizeof(size_t) + len);
    
    size_t count = 0;
    
    ent_condition *list = (ent_condition*)(data + 2 * sizeof(size_t));
    while(len) {
        list[count] = va_arg(args, ent_condition);
        ++count;
        len -= sizeof(ent_condition);
    }
    
    *(size_t*)(data + sizeof(size_t)) = count;
    
    size_t i = min;
    for(;;) {
        if(entity_pool[i]) {
            for(size_t j = 0; j < count; ++j) {
                if(!list[j].compatible(entity_pool[i], list[j].data)) {
                    goto incompatible;
                }
            }
            break;
            incompatible: { }
        }
        ++i;
        if(i == zsize(entity_pool)) break;
    }
    
    *(size_t*)(data) = i;
    
    return (ent_iterator){ &ent_all_next, /*&ent_all_has_next,*/ &ent_all_cleanup, data };
}

void radius_cleanup(void *data) {
    free(data);
}

int radius_compatible(entity *e, void *data) {
    vec *v = (vec*)data;
    float *r = (float*)(data + sizeof(vec));
    float test = len2(vsub(e->position, *v));
    return test <= (*r) * (*r);
}

ent_condition within_radius(vec v, float r) {
    void *data = malloc(sizeof(vec) + sizeof(float));
    *(vec*)(data) = v;
    *(float*)(data + sizeof(vec)) = r;
    
    return (ent_condition){ &radius_compatible, &radius_cleanup, data };
}

void not_entity_cleanup(void *data) { }

int not_entity_compatible(entity *e, void *data) {
    entity *check = (entity*)data;
    return check != e;
}

ent_condition not_entity(entity *e) {
    return (ent_condition){ &not_entity_compatible, &not_entity_cleanup, e };
}

void tagged_cleanup(void *data) {
    free(data);
}

int tagged_compatible(entity *e, void *data) {
    int tag = *(int*)(data);
    return e->tag == tag;
}

ent_condition tagged(int tag) {
    int *data = malloc(sizeof(int));
    *data = tag;
    
    return (ent_condition){ &tagged_compatible, &tagged_cleanup, data };
}

void is_not_cleanup(void *data) {
    ent_condition c = *(ent_condition*)data;
    c.cleanup(c.data);
    free(data);
}

int is_not_compatible(entity *e, void *data) {
    ent_condition c = *(ent_condition*)data;
    
    return !(c.compatible(e, c.data));
}

ent_condition is_not(ent_condition c) {
    ent_condition *data = malloc(sizeof(ent_condition));
    *data = c;
    
    return (ent_condition){ &is_not_compatible, &is_not_cleanup, data };
}

void with_component_cleanup(void *data) {
    free(data);
}

int with_component_compatible(entity *e, void *data) {
    c_table tocheck = *(c_table*)(data);
    
    for(size_t i = 0; i < zsize(e->components); ++i) {
        if(c_tabl(e->components[i]) == tocheck) return 1;
    }
    
    return 0;
}

ent_condition with_component(c_table ct) {
    c_table *data = malloc(sizeof(c_table));
    *data = ct;
    
    return (ent_condition){ &with_component_compatible, &with_component_cleanup, data };
}

void any_cleanup(void *data) { }

int any_compatible(entity *e, void *data) { return 1; }

ent_condition any(void) {
    return (ent_condition){ &any_compatible, &any_cleanup, NULL };
}

component get_component_(entity *e, c_table ct) {
    for(size_t i = 0; i < zsize(e->components); ++i) {
        if(c_tabl(e->components[i]) == ct) return e->components[i];
    }
    return NULL;
}

void add_component(entity *e, component c) {
    zlist_add(e->components, c);
    c_tabl(c)->parent = e;
}