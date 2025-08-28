#include "bml-string.h"

void init_bml_string(BmlString* bml_string, const char* text) {
    strncpy(bml_string->text, text, MKSBMLI_MAX_TEXT_LENGTH - 1);
}

void dump_bml_string(BmlString* bml_string) {
    printf("BML_NUMBER#text[%s]\n", bml_string->text);
}
