#include <stdio.h>

#include "vanish.h"

void init_vanish(Vanish* vanish, BulletmlBase* parent) {
    vanish->type = BULLETML_ELEMENT_TYPE_VANISH;
    vanish->parent = parent;
}

void dump_vanish(Vanish* vanish) {
    printf("VANISH\n");
}
