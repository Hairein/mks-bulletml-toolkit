#ifndef BML_STRING_H
#define BML_STRING_H

#include "mksbmli-defines.h"

typedef struct {
    char text[MKSBMLI_MAX_TEXT_LENGTH];
} BmlString;

void init_bml_string(BmlString* bml_string, const char* text);

void dump_bml_string(BmlString* bml_string);

#endif // BML_STRING_H
