#include <stdio.h>

#include "times.h"

void init_times(Times* times, BulletmlBase* parent, const char* number) {
    times->type = BULLETML_ELEMENT_TYPE_TIMES;
    times->parent = parent;

    init_bml_number(&times->contents, number);
}

void dump_times(Times* times) {
    printf("TIMES#contents[%s]\n", times->contents.expression);
}
