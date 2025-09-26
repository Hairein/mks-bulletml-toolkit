#ifndef VANISH_H
#define VANISH_H

#include "mksbmli-defines.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} Vanish;

void init_vanish(Vanish* vanish, BulletmlBase* parent);

void dump_vanish(Vanish* vanish);

#endif // VANISH_H
