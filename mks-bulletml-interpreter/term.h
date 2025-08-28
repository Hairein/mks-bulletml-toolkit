#ifndef TERM_H
#define TERM_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;
} Term;

void init_term(Term* term, BulletmlBase* parent);

void dump_term(Term* term);

#endif // TERM_H
