#include "entity.h"
#include "new.h"

zlist_of(entity*) entity_pool;

size_t minimum_empty = 0;

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
    zlist_init(ent->components, 0);
    
    for(size_t i = minimum_empty; i < zsize(entity_pool); ++i) {
        if(!entity_pool[i]) {
            entity_pool[i] = ent;
            minimum_empty = i + 1;
            return ent;
        }
    }
    minimum_empty = zsize(entity_pool);
    zlist_add(entity_pool, ent);
    return ent;
}

void ent_destroy(entity *e) {
    if(!e) return;
    
    for(size_t i = 0; i < zsize(entity_pool); ++i) {
        if(entity_pool[i] == e) {
            minimum_empty = i;
            entity_pool[i] = NULL;
            break;
        }
    }
    
    zfree(e->components);
    free(e);
}