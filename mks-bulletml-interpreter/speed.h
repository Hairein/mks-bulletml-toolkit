#ifndef SPEED_H
#define SPEED_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    ARS_TYPE attribute;
} Speed;

void init_speed(Speed* speed, BulletmlBase* parent, ARS_TYPE attribute);

void dump_speed(Speed* speed);

#endif // SPEED_H
