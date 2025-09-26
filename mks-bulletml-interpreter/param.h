#ifndef PARAM_H
#define PARAM_H

#include "mksbmli-defines.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    BmlNumber contents;
} Param;

void init_param(Param* param, BulletmlBase* parent, const char* contents);

void dump_param(Param* param);

#endif // PARAM_H
