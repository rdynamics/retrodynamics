#include "entity.h"
#include "component.h"

void init() {

}

void loop() {    
    for_ent(e, ent_all(any()), {
        for(size_t i = 0; i < zsize(e->components); ++i) {
            c_tick(e->components[i]);
        }
    })
}