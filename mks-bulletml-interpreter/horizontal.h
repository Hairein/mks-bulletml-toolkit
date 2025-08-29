#ifndef HORIZONTAL_H
#define HORIZONTAL_H

#include "mksbmli-defines.h"
#include "bulletml-base.h"
#include "bml-number.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    ARS_TYPE attribute;
    BmlNumber contents;
} Horizontal;

void init_horizontal(Horizontal* horizontal, BulletmlBase* parent, ARS_TYPE attribute, const char* acceleration);

void dump_horizontal(Horizontal* horizontal);

#endif // HORIZONTAL_H
