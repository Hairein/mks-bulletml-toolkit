#include "utility.h"
#include "param.h"

void init_param(Param* param, BulletmlBase* parent) {
    param->type = BULLETML_ELEMENT_TYPE_PARAM;
    param->parent = parent;
}

void dump_param(Param* param) {
    printf("PARAM\n");
}
