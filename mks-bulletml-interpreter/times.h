#ifndef TIMES_H
#define TIMES_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} Times;

void init_times(Times* times, BulletmlBase* parent);

void dump_times(Times* times);

#endif // TIMES_H
