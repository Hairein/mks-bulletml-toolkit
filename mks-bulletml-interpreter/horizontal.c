#include "utility.h"
#include "horizontal.h"

void init_horizontal(Horizontal* horizontal, BulletmlBase* parent, ARS_TYPE attribute) {
    horizontal->type = BULLETML_ELEMENT_TYPE_HORIZONTAL;
    horizontal->parent = parent;

    horizontal->attribute = attribute;
}

void dump_horizontal(Horizontal* horizontal) {
    char type[MKSBMLI_MAX_TEXT_LENGTH] = "absolute";
    get_ars_type_text(horizontal->attribute, type);

    printf("HORIZONTAL type(%s)\n", type);
}
