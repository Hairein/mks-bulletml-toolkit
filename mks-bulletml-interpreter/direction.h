#ifndef DIRECTION_H
#define DIRECTION_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"
#include "bml-number.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    AARS_TYPE attribute;
    BmlNumber contents;
} Direction;

void init_direction(Direction* direction, BulletmlBase* parent, AARS_TYPE attribute, const char* degrees);

void dump_direction(Direction* direction);

#endif // DIRECTION_H
