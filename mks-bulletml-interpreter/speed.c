#include "utility.h"
#include "speed.h"

void init_speed(Speed* speed, BulletmlBase* parent, ARS_TYPE attribute) {
    speed->type = BULLETML_ELEMENT_TYPE_SPEED;
    speed->parent = parent;

    speed->attribute = attribute;
}

void dump_speed(Speed* speed) {
    char type[MKSBMLI_MAX_TEXT_LENGTH];
    get_ars_type_text(speed->attribute, type);

    printf("SPEED type(%s)\n", type);
}
