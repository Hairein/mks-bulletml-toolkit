#ifndef BULLETML_BASE_H
#define BULLETML_BASE_H

#include "mks-bulletml-interpreter.h"

typedef struct BulletmlBase BulletmlBase;

struct BulletmlBase {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
};

void init_bulletml_base(BulletmlBase* base, BULLETML_ELEMENT_TYPE type, BulletmlBase* parent);

#endif // BULLETML_BASE_H
