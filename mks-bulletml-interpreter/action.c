#include "action.h"

void init_action(Action* action, BulletmlBase* parent, const char* label) {
    action->type = BULLETML_ELEMENT_TYPE_ACTION;
    action->parent = parent;

    strncpy(action->label, label, MKSBMLI_MAX_TEXT_LENGTH);
}

void dump_action(Action* action) {
    printf("BULLETML label(%s)\n", action->label);
}
