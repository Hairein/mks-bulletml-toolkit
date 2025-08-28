#ifndef TERM_H
#define TERM_H

#include "mks-bulletml-interpreter.h"
#include "bulletml-base.h"
#include "bml-number.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    BmlNumber contents;
} Term;

void init_term(Term* term, BulletmlBase* parent, const char* number);

void dump_term(Term* term);

#endif // TERM_H
