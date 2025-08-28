#include "utility.h"
#include "speed.h"

void init_speed(Speed* speed, BulletmlBase* parent, ARS_TYPE attribute, const char* velocity) {
    speed->type = BULLETML_ELEMENT_TYPE_SPEED;
    speed->parent = parent;

    speed->attribute = attribute;

    init_bml_number(&speed->contents, velocity);
}

void dump_speed(Speed* speed) {
    char type[MKSBMLI_MAX_TEXT_LENGTH];
    get_ars_type_text(speed->attribute, type);

    printf("SPEED type(%s) contents(%s)\n", type, speed->contents.expression);
}
