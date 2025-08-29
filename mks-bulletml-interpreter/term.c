#include <stdio.h>

#include "term.h"

void init_term(Term* term, BulletmlBase* parent, const char* frames) {
    term->type = BULLETML_ELEMENT_TYPE_TERM;
    term->parent = parent;

    init_bml_number(&term->contents, frames);
}

void dump_term(Term* term) {
    printf("TERM#contents[%s]\n", term->contents.expression);
}
