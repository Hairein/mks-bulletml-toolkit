#ifndef BULLETML_H
#define BULLETML_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    BULLETML_ATTRIBUTE_TYPE attribute;
} Bulletml;

void init_bulletml(Bulletml* bulletml, BulletmlBase* parent, BULLETML_ATTRIBUTE_TYPE attribute);

void dump_bulletml(Bulletml* bulletml);

#endif // BULLETML_H
