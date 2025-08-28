#ifndef FIRE_REF_H
#define FIRE_REF_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    char label[MKSBMLI_MAX_TEXT_LENGTH];
} FireRef;

void init_fire_ref(FireRef* fire_ref, BulletmlBase* parent, const char* label);

void dump_fire_ref(FireRef* fire_ref);

#endif // FIRE_REF_H
