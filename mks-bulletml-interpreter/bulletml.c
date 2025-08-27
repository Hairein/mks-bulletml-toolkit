#include "utility.h"
#include "bulletml.h"

void init_bulletml(Bulletml* bulletml, BulletmlBase* parent, BULLETML_ATTRIBUTE_TYPE attribute) {
    bulletml->type = BULLETML_ELEMENT_TYPE_BULLETML;
    bulletml->parent = parent;

    bulletml->attribute = attribute;
}

void dump_bulletml(Bulletml* bulletml) {
    char type[MKSBMLI_MAX_TEXT_LENGTH] = "none";
    switch(bulletml->attribute) {
    case BULLETML_ATTRIBUTE_TYPE_VERTICAL: copy_text(type, "vertical"); break;
    case BULLETML_ATTRIBUTE_TYPE_HORIZONTAL: copy_text(type, "horizontal"); break;
    default: break;
    }

    printf("BULLETML type(%s)\n", type);
}
