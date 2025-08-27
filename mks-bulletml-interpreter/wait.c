#include "wait.h"

void init_wait(Wait* wait, BulletmlBase* parent) {
    wait->type = BULLETML_ELEMENT_TYPE_WAIT;
    wait->parent = parent;
}

void dump_wait(Wait* wait) {
    printf("WAIT\n");
}
