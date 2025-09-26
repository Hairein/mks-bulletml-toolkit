#ifndef CHANGE_SPEED_H
#define CHANGE_SPEED_H

#include "mksbmli-defines.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} ChangeSpeed;

void init_change_speed(ChangeSpeed* change_speed, BulletmlBase* parent);

void dump_change_speed(ChangeSpeed* change_speed);

#endif // CHANGE_SPEED_H
