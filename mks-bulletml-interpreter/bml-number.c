#include <string.h>
#include <stdio.h>

#include "tinyexpr.h"
#include "utility.h"
#include "bml-number.h"

void init_bml_number(BmlNumber* bml_number, const char* expression) {
    strncpy(bml_number->expression, expression, MKSBMLI_MAX_TEXT_LENGTH - 1);
}

void dump_bml_number(BmlNumber* bml_number) {
    printf("BML_NUMBER#expression[%s]\n", bml_number->expression);
}

int evaluate_bml_number_as_int(BmlNumber* bml_number, float rank, float* params, int nos_params) {
    char temp[MKSBMLI_MAX_TEXT_LENGTH];
    replace_rand_keyword(temp, bml_number->expression);
    char temp2[MKSBMLI_MAX_TEXT_LENGTH];
    replace_rank_keyword(rank, temp2, temp);
    char temp3[MKSBMLI_MAX_TEXT_LENGTH];
    replace_parameters_keyword(params, nos_params, temp3, temp2);

    int error;
    double result = te_interp(temp3, &error);
    if(error == 0) {
        return (int)result;
    }

    return 0;
}

unsigned int evaluate_bml_number_as_unsigned_int(BmlNumber* bml_number, float rank, float* params, int nos_params) {
    char temp[MKSBMLI_MAX_TEXT_LENGTH];
    replace_rand_keyword(temp, bml_number->expression);
    char temp2[MKSBMLI_MAX_TEXT_LENGTH];
    replace_rank_keyword(rank, temp2, temp);
    char temp3[MKSBMLI_MAX_TEXT_LENGTH];
    replace_parameters_keyword(params, nos_params, temp3, temp2);

    int error;
    double result = te_interp(temp3, &error);
    if(error == 0) {
        return (unsigned int)result;
    }

    return 0;
}

float evaluate_bml_number_as_float(BmlNumber* bml_number, float rank, float* params, int nos_params) {
    char temp[MKSBMLI_MAX_TEXT_LENGTH];
    replace_rand_keyword(temp, bml_number->expression);
    char temp2[MKSBMLI_MAX_TEXT_LENGTH];
    replace_rank_keyword(rank, temp2, temp);
    char temp3[MKSBMLI_MAX_TEXT_LENGTH];
    replace_parameters_keyword(params, nos_params, temp3, temp2);

    int error;
    double result = te_interp(temp3, &error);
    if(error == 0) {
        return (float)result;
    }

    return 0.0f;
}
