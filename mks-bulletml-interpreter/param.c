#include <stdio.h>

#include "bml-number.h"
#include "param.h"

void init_param(Param* param, BulletmlBase* parent, const char* contents) {
    param->type = BULLETML_ELEMENT_TYPE_PARAM;
    param->parent = parent;

    init_bml_number(&param->contents, contents);
}

void dump_param(Param* param) {
    printf("PARAM#contents[%s]\n", param->contents.expression);
}
