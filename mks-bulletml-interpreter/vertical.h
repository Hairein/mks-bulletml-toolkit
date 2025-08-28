#ifndef VERTICAL_H
#define VERTICAL_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"
#include "bml-number.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    ARS_TYPE attribute;
    BmlNumber contents;
} Vertical;

void init_vertical(Vertical* vertical, BulletmlBase* parent, ARS_TYPE attribute, const char* acceleration);

void dump_vertical(Vertical* vertical);

#endif // VERTICAL_H
