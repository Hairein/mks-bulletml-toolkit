#ifndef BML_NUMBER_H
#define BML_NUMBER_H

#include "mksbmli-defines.h"

typedef struct {
    char expression[MKSBMLI_MAX_TEXT_LENGTH];
} BmlNumber;

void init_bml_number(BmlNumber* bml_number, const char* expression);

void dump_bml_number(BmlNumber* bml_number);

int evaluate_bml_number_as_int(BmlNumber* bml_number, float rank, float* params, int nos_params);
unsigned int evaluate_bml_number_as_unsigned_int(BmlNumber* bml_number, float rank, float* params, int nos_params);
float evaluate_bml_number_as_float(BmlNumber* bml_number, float rank, float* params, int nos_params);

#endif // BML_NUMBER_H
