#include "tinyexpr.h"
#include "bml-number.h"

void init_bml_number(BmlNumber* bml_number, const char* expression) {
    strncpy(bml_number->expression, expression, MKSBMLI_MAX_TEXT_LENGTH - 1);
}

void dump_bml_number(BmlNumber* bml_number) {
    printf("BML_NUMBER expression(%s)\n", bml_number->expression);
}

int evaluate_bml_number_as_int(BmlNumber* bml_number) {
    int error;
    double result = te_interp(bml_number->expression, &error);
    if(error == 0) {
        return (int)result;
    }

    return 0;
}

unsigned int evaluate_bml_number_as_unsigned_int(BmlNumber* bml_number) {
    int error;
    double result = te_interp(bml_number->expression, &error);
    if(error == 0) {
        return (unsigned int)result;
    }

    return 0;
}

float evaluate_bml_number_as_float(BmlNumber* bml_number) {
    int error;
    double result = te_interp(bml_number->expression, &error);
    if(error == 0) {
        return (float)result;
    }

    return 0.0f;
}
