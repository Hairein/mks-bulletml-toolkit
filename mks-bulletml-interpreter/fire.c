#include "utility.h"
#include "fire.h"

void init_fire(Fire* fire, BulletmlBase* parent, const char* label) {
    fire->type = BULLETML_ELEMENT_TYPE_FIRE;
    fire->parent = parent;

    copy_text(fire->label, label);
}

void dump_fire(Fire* fire) {
    printf("FIRE#label[%s]\n", fire->label);
}
