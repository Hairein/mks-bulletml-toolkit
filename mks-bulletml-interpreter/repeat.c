#include <stdio.h>

#include "repeat.h"

void init_repeat(Repeat* repeat, BulletmlBase* parent) {
    repeat->type = BULLETML_ELEMENT_TYPE_REPEAT;
    repeat->parent = parent;
}

void dump_repeat(Repeat* repeat) {
    printf("REPEAT\n");
}
