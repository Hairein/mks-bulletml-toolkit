#include "wait.h"

void init_wait(Wait* wait, BulletmlBase* parent, const char* expression) {
    wait->type = BULLETML_ELEMENT_TYPE_WAIT;
    wait->parent = parent;

    init_bml_number(&wait->content, expression);
}

void dump_wait(Wait* wait) {
    printf("WAIT expression(%s)\n", wait->content.expression);
}
