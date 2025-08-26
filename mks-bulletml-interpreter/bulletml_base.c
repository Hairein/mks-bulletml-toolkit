#include "bulletml_base.h"

void init_bulletml_base(BulletmlBase* base, BULLETML_ELEMENT_TYPE type, BulletmlBase* parent) {
    base->type = type;
    base->parent = parent;
}
