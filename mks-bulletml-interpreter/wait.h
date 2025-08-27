#ifndef WAIT_H
#define WAIT_H

#include "mks-bulletml-interpreter.h"
#include "bulletml_base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} Wait;

void init_wait(Wait* wait, BulletmlBase* parent);

void dump_wait(Wait* wait);

#endif // WAIT_H
