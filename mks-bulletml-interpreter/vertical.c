#include "utility.h"
#include "vertical.h"

void init_vertical(Vertical* vertical, BulletmlBase* parent, ARS_TYPE attribute, const char* acceleration) {
    vertical->type = BULLETML_ELEMENT_TYPE_VERTICAL;
    vertical->parent = parent;

    vertical->attribute = attribute;

    init_bml_number(&vertical->contents, acceleration);
}

void dump_vertical(Vertical* vertical) {
    char type[MKSBMLI_MAX_TEXT_LENGTH];
    get_ars_type_text(vertical->attribute, type);

    printf("VERTICAL#type[%s]|contents[%s]\n", type, vertical->contents.expression);
}
