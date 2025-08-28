#include "utility.h"
#include "vertical.h"

void init_vertical(Vertical* vertical, BulletmlBase* parent, ARS_TYPE attribute) {
    vertical->type = BULLETML_ELEMENT_TYPE_VERTICAL;
    vertical->parent = parent;

    vertical->attribute = attribute;
}

void dump_vertical(Vertical* vertical) {
    char type[MKSBMLI_MAX_TEXT_LENGTH] = "absolute";
    get_ars_type_text(vertical->attribute, type);

    printf("VERTICAL type(%s)\n", type);
}
