#ifndef ACTION_REF_H
#define ACTION_REF_H

#include "mksbmli-defines.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    char label[MKSBMLI_MAX_TEXT_LENGTH];
} ActionRef;

void init_action_ref(ActionRef* action_ref, BulletmlBase* parent, const char* label);

void dump_action_ref(ActionRef* action_ref);

#endif // ACTION_REF_H
