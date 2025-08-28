#ifndef VERTICAL_H
#define VERTICAL_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    ARS_TYPE attribute;
} Vertical;

void init_vertical(Vertical* vertical, BulletmlBase* parent, ARS_TYPE attribute);

void dump_vertical(Vertical* vertical);

#endif // VERTICAL_H
