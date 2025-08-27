#ifndef FIRE_H
#define FIRE_H

#include "mks-bulletml-interpreter.h"
#include "bulletml_base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    char label[MKSBMLI_MAX_TEXT_LENGTH];
} Fire;

void init_fire(Fire* fire, BulletmlBase* parent, const char* label);

void dump_fire(Fire* fire);

#endif // FIRE_H
