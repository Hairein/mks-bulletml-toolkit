#include "utility.h"
#include "horizontal.h"

void init_horizontal(Horizontal* horizontal, BulletmlBase* parent, ARS_TYPE attribute) {
    horizontal->type = BULLETML_ELEMENT_TYPE_HORIZONTAL;
    horizontal->parent = parent;

    horizontal->attribute = attribute;
}

void dump_horizontal(Horizontal* horizontal) {
    char type[MKSBMLI_MAX_TEXT_LENGTH] = "absolute";
    switch(horizontal->attribute) {
    case ARS_TYPE_RELATIVE: copy_text(type, "relative"); break;
    case ARS_TYPE_SEQUENCE: copy_text(type, "sequence"); break;
    case ARS_TYPE_ABSOLUTE:
    default: break;
    }

    printf("HORIZONTAL type(%s)\n", type);
}
