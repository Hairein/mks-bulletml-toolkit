#ifndef ACTION_H
#define ACTION_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    char label[MKSBMLI_MAX_TEXT_LENGTH];
} Action;

void init_action(Action* action, BulletmlBase* parent, const char* label);

void dump_action(Action* action);

#endif // ACTION_H
