#include "accel.h"

void init_accel(Accel* accel, BulletmlBase* parent) {
    accel->type = BULLETML_ELEMENT_TYPE_ACCEL;
    accel->parent = parent;
}

void dump_accel(Accel* accel) {
    printf("ACCEL\n");
}
