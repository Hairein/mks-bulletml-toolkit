#include "change_speed.h"

void init_change_speed(ChangeSpeed* change_speed, BulletmlBase* parent) {
    change_speed->type = BULLETML_ELEMENT_TYPE_CHANGE_SPEED;
    change_speed->parent = parent;
}

void dump_change_speed(ChangeSpeed* change_speed) {
    printf("CHANGE_SPEED\n");
}
