#include "bulletml.h"

void init_bulletml(Bulletml* bulletml, BulletmlBase* parent, BULLETML_ATTRIBUTE_TYPE attribute) {
    bulletml->type = BULLETML_ELEMENT_TYPE_BULLETML;
    bulletml->parent = parent;

    bulletml->attribute = attribute;
}

void dump_bulletml(Bulletml* bulletml) {
    char type[128] = "none";
    switch(bulletml->attribute) {
    case BULLETML_ATTRIBUTE_TYPE_VERTICAL: strncpy(type, "vertical", MKSBMLI_MAX_TEXT_LENGTH); break;
    case BULLETML_ATTRIBUTE_TYPE_HORIZONTAL: strncpy(type, "horizontal", MKSBMLI_MAX_TEXT_LENGTH); break;
    default: break;
    }

    printf("BULLETML type(%s)\n", type);
}
