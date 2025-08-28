#ifndef TIMES_H
#define TIMES_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"
#include "bml-number.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    BmlNumber contents;
} Times;

void init_times(Times* times, BulletmlBase* parent, const char* number);

void dump_times(Times* times);

#endif // TIMES_H
