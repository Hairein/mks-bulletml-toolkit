#ifndef HORIZONTAL_H
#define HORIZONTAL_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    ARS_TYPE attribute;
} Horizontal;

void init_horizontal(Horizontal* horizontal, BulletmlBase* parent, ARS_TYPE attribute);

void dump_horizontal(Horizontal* horizontal);

#endif // HORIZONTAL_H
