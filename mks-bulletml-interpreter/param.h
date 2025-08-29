#ifndef PARAM_H
#define PARAM_H

#include "mksbmli-defines.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} Param;

void init_param(Param* param, BulletmlBase* parent);

void dump_param(Param* param);

#endif // PARAM_H
