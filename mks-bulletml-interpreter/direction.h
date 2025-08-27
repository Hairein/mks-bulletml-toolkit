#ifndef DIRECTION_H
#define DIRECTION_H

#include "mks-bulletml-interpreter.h"
#include "bulletml_base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    DIRECTION_TYPE attribute;
} Direction;

void init_direction(Direction* direction, BulletmlBase* parent, DIRECTION_TYPE attribute);

void dump_direction(Direction* direction);

#endif // DIRECTION_H
