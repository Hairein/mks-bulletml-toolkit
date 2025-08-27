#ifndef SPEED_H
#define SPEED_H

#include "mks-bulletml-interpreter.h"
#include "bulletml_base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    SPEED_TYPE attribute;
} Speed;

void init_speed(Speed* speed, BulletmlBase* parent, SPEED_TYPE attribute);

void dump_speed(Speed* speed);

#endif // SPEED_H
