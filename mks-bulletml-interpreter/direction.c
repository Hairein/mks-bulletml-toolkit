#include "utility.h"
#include "direction.h"

void init_direction(Direction* direction, BulletmlBase* parent, AARS_TYPE attribute) {
    direction->type = BULLETML_ELEMENT_TYPE_DIRECTION;
    direction->parent = parent;

    direction->attribute = attribute;
}

void dump_direction(Direction* direction) {
    char type[MKSBMLI_MAX_TEXT_LENGTH];
    get_aars_type_text(direction->attribute, type);

    printf("DIRECTION type(%s)\n", type);
}
