#include <stdio.h>

#include "change-direction.h"

void init_change_direction(ChangeDirection* change_direction, BulletmlBase* parent) {
    change_direction->type = BULLETML_ELEMENT_TYPE_CHANGE_DIRECTION;
    change_direction->parent = parent;
}

void dump_change_direction(ChangeDirection* change_direction) {
    printf("CHANGE_DIRECTION\n");
}
