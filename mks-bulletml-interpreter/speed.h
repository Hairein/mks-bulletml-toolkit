#ifndef SPEED_H
#define SPEED_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"
#include "bml-number.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    ARS_TYPE attribute;
    BmlNumber contents;
} Speed;

void init_speed(Speed* speed, BulletmlBase* parent, ARS_TYPE attribute, const char* velocity);

void dump_speed(Speed* speed);

#endif // SPEED_H
