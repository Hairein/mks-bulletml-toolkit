#include "utility.h"
#include "fire-ref.h"

void init_fire_ref(FireRef* fire_ref, BulletmlBase* parent, const char* label) {
    fire_ref->type = BULLETML_ELEMENT_TYPE_FIRE_REF;
    fire_ref->parent = parent;

    copy_text(fire_ref->label, label);
}

void dump_fire_ref(FireRef* fire_ref) {
    printf("FIRE_REF label(%s)\n", fire_ref->label);
}
