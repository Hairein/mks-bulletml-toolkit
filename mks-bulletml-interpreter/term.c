#include "utility.h"
#include "term.h"

void init_term(Term* term, BulletmlBase* parent) {
    term->type = BULLETML_ELEMENT_TYPE_TERM;
    term->parent = parent;
}

void dump_term(Term* term) {
    printf("TERM\n");
}
