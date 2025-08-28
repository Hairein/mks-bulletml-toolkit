#ifndef CHANGE_DIRECTION_H
#define CHANGE_DIRECTION_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} ChangeDirection;

void init_change_direction(ChangeDirection* change_direction, BulletmlBase* parent);

void dump_change_direction(ChangeDirection* change_direction);

#endif // CHANGE_DIRECTION_H
