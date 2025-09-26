#include "utility.h"
#include "horizontal.h"

void init_horizontal(Horizontal* horizontal, BulletmlBase* parent, ARS_TYPE attribute, const char* acceleration) {
    horizontal->type = BULLETML_ELEMENT_TYPE_HORIZONTAL;
    horizontal->parent = parent;

    horizontal->attribute = attribute;

    init_bml_number(&horizontal->contents, acceleration);
}

void dump_horizontal(Horizontal* horizontal) {
    char type[MKSBMLI_MAX_TEXT_LENGTH] = "absolute";
    get_ars_type_text(horizontal->attribute, type);

    printf("HORIZONTAL#type[%s]|contents[%s]\n", type, horizontal->contents.expression);
}
