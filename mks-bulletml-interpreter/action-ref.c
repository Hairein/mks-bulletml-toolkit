#include "utility.h"
#include "action-ref.h"

void init_action_ref(ActionRef* action_ref, BulletmlBase* parent, const char* label) {
    action_ref->type = BULLETML_ELEMENT_TYPE_ACTION_REF;
    action_ref->parent = parent;

    copy_text(action_ref->label, label);
}

void dump_action_ref(ActionRef* action_ref) {
    printf("ACTION_REF#label[%s]\n", action_ref->label);
}
