#include "utility.h"
#include "speed.h"

void init_speed(Speed* speed, BulletmlBase* parent, SPEED_TYPE attribute) {
    speed->type = BULLETML_ELEMENT_TYPE_SPEED;
    speed->parent = parent;

    speed->attribute = attribute;
}

void dump_speed(Speed* speed) {
    char type[MKSBMLI_MAX_TEXT_LENGTH] = "absolute";
    switch(speed->attribute) {
    case SPEED_TYPE_RELATIVE: copy_text(type, "relative"); break;
    case SPEED_TYPE_SEQUENCE: copy_text(type, "sequence"); break;
    case SPEED_TYPE_ABSOLUTE:
    default: break;
    }

    printf("SPEED type(%s)\n", type);
}
