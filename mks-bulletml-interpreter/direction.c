#include "utility.h"
#include "direction.h"

void init_direction(Direction* direction, BulletmlBase* parent, DIRECTION_TYPE attribute) {
    direction->type = BULLETML_ELEMENT_TYPE_DIRECTION;
    direction->parent = parent;

    direction->attribute = attribute;
}

void dump_direction(Direction* direction) {
    char type[MKSBMLI_MAX_TEXT_LENGTH] = "absolute";
    switch(direction->attribute) {
    case DIRECTION_TYPE_AIM: copy_text(type, "aim"); break;
    case DIRECTION_TYPE_RELATIVE: copy_text(type, "relative"); break;
    case DIRECTION_TYPE_SEQUENCE: copy_text(type, "sequence"); break;
    case DIRECTION_TYPE_ABSOLUTE:
    default: break;
    }

    printf("DIRECTION type(%s)\n", type);
}
