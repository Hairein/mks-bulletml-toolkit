#include "utility.h"
#include "action.h"

void init_action(Action* action, BulletmlBase* parent, const char* label) {
    action->type = BULLETML_ELEMENT_TYPE_ACTION;
    action->parent = parent;

    copy_text(action->label, label);
}

void dump_action(Action* action) {
    printf("ACTION label(%s)\n", action->label);
}
