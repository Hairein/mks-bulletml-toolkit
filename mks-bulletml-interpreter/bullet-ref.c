#include "utility.h"
#include "bullet-ref.h"

void init_bullet_ref(BulletRef* bullet_ref, BulletmlBase* parent, const char* label) {
    bullet_ref->type = BULLETML_ELEMENT_TYPE_BULLET_REF;
    bullet_ref->parent = parent;

    copy_text(bullet_ref->label, label);
}

void dump_bullet_ref(BulletRef* bullet_ref) {
    printf("BULLET_REF label(%s)\n", bullet_ref->label);
}
