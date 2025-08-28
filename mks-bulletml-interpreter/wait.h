#ifndef WAIT_H
#define WAIT_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"
#include "bml-number.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    BmlNumber content;
} Wait;

void init_wait(Wait* wait, BulletmlBase* parent, const char* expression);

void dump_wait(Wait* wait);

#endif // WAIT_H
