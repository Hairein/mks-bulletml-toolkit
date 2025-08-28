#include "utility.h"
#include "times.h"

void init_times(Times* times, BulletmlBase* parent) {
    times->type = BULLETML_ELEMENT_TYPE_TIMES;
    times->parent = parent;
}

void dump_times(Times* times) {
    printf("TIMES\n");
}
