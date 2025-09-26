#include "utility.h"
#include "bulletml.h"

void init_bulletml(Bulletml* bulletml, BulletmlBase* parent, BULLETML_ATTRIBUTE_TYPE attribute) {
    bulletml->type = BULLETML_ELEMENT_TYPE_BULLETML;
    bulletml->parent = parent;

    bulletml->attribute = attribute;
}

void dump_bulletml(Bulletml* bulletml) {
    char type[MKSBMLI_MAX_TEXT_LENGTH];
    get_bulletml_attribute_type_text(bulletml->attribute, type);

    printf("BULLETML#type[%s]\n", type);
}
