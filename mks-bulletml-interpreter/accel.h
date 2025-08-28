#ifndef ACCEL_H
#define ACCEL_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} Accel;

void init_accel(Accel* accel, BulletmlBase* parent);

void dump_accel(Accel* accel);

#endif // ACCEL_H
