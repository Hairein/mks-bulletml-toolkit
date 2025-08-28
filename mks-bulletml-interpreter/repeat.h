#ifndef REPEAT_H
#define REPEAT_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} Repeat;

void init_repeat(Repeat* repeat, BulletmlBase* parent);

void dump_repeat(Repeat* repeat);

#endif // REPEAT_H
