#ifndef BULLET_REF_H
#define BULLET_REF_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    char label[MKSBMLI_MAX_TEXT_LENGTH];
} BulletRef;

void init_bullet_ref(BulletRef* bullet_ref, BulletmlBase* parent, const char* label);

void dump_bullet_ref(BulletRef* bullet_ref);

#endif // BULLET_REF_H
