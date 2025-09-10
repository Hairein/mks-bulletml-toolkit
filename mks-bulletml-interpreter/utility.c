#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "platform-random.h"
#include "utility.h"

int extract_xml_property_text(xmlNode* node, char* name, char* text) {
    xmlChar *xml_text = xmlGetProp(node, (const xmlChar *)name);
    if (xml_text) {
        copy_text(text, (const char*)xml_text);
        xmlFree(xml_text);

        return MKSBMLI_NO_ERROR;
    }

    return MKSBMLI_XML_PARSE_ERROR;
}

void extract_xml_property_label_text(xmlNode* node, char* text) {
    copy_text(text, "");

    char entry[MKSBMLI_MAX_TEXT_LENGTH];
    if(extract_xml_property_text(node, "label", entry) == MKSBMLI_NO_ERROR) {
        strncpy(text, entry, MKSBMLI_MAX_TEXT_LENGTH - 1);
    }
}

void extract_xml_property_bulletml_type(xmlNode* node, BULLETML_ATTRIBUTE_TYPE* type) {
    *type = BULLETML_ATTRIBUTE_TYPE_HORIZONTAL;

    char entry[MKSBMLI_MAX_TEXT_LENGTH];
    if(extract_xml_property_text(node, "type", entry) == MKSBMLI_NO_ERROR) {
        get_bulletml_attribute_type(entry, type);
    }
}

void extract_xml_property_aars_type(xmlNode* node, AARS_TYPE* type) {
    *type = AARS_TYPE_ABSOLUTE;

    char entry[MKSBMLI_MAX_TEXT_LENGTH];
    if(extract_xml_property_text(node, "type", entry) == MKSBMLI_NO_ERROR) {
        get_aars_type(entry, type);
    }
}

void extract_xml_property_ars_type(xmlNode* node, ARS_TYPE* type) {
    *type = ARS_TYPE_ABSOLUTE;

    char entry[MKSBMLI_MAX_TEXT_LENGTH];
    if(extract_xml_property_text(node, "type", entry) == MKSBMLI_NO_ERROR) {
        get_ars_type(entry, type);
    }
}

void extract_xml_text_content(xmlNode* node, char content[MKSBMLI_MAX_TEXT_LENGTH], const char* default_text) {
    memset(content, 0, MKSBMLI_MAX_TEXT_LENGTH);
    strncpy(content, default_text, MKSBMLI_MAX_TEXT_LENGTH - 1);

    if (node && node->children && node->children->type == XML_TEXT_NODE) {
        xmlChar* child_content = node->children->content;
        if (child_content) {
            memset(content, 0, MKSBMLI_MAX_TEXT_LENGTH);
            copy_text(content, (const char*)child_content);
        }
    }
}

void copy_filename(char* target, const char* source) {
    memset(target, 0, MKSBMLI_XML_FILENAME_MAX_LENGTH);
    strncpy(target, source, MKSBMLI_XML_FILENAME_MAX_LENGTH - 1);
}

void copy_text(char* target, const char* source) {
    memset(target, 0, MKSBMLI_MAX_TEXT_LENGTH);
    strncpy(target, source, MKSBMLI_MAX_TEXT_LENGTH - 1);
}

int compare_texts(const char* text1, const char* text2) {
    return strncmp(text1, text2, MKSBMLI_MAX_TEXT_LENGTH);
}

int get_bulletml_attribute_type(const char* text, BULLETML_ATTRIBUTE_TYPE* type) {
    if(compare_texts(text, "none") ==  0) *type = BULLETML_ATTRIBUTE_TYPE_NONE;
    if(compare_texts(text, "vertical") ==  0) *type = BULLETML_ATTRIBUTE_TYPE_VERTICAL;
    else if(compare_texts(text, "horizontal") ==  0) *type = BULLETML_ATTRIBUTE_TYPE_HORIZONTAL;
    else return  MKSBMLI_INVALID_PARAMETER;

    return MKSBMLI_NO_ERROR;
}

int get_bulletml_attribute_type_text(BULLETML_ATTRIBUTE_TYPE type, char* text) {
    switch(type) {
    case BULLETML_ATTRIBUTE_TYPE_NONE: copy_text(text, "none"); break;
    case BULLETML_ATTRIBUTE_TYPE_VERTICAL: copy_text(text, "vertical"); break;
    case BULLETML_ATTRIBUTE_TYPE_HORIZONTAL: copy_text(text, "horizontal"); break;
    default: return MKSBMLI_INVALID_PARAMETER;
    }

    return MKSBMLI_NO_ERROR;
}

int get_aars_type(const char* text, AARS_TYPE* type) {
    if(compare_texts(text, "aim") ==  0) *type = AARS_TYPE_AIM;
    if(compare_texts(text, "absolute") ==  0) *type = AARS_TYPE_ABSOLUTE;
    else if(compare_texts(text, "relative") ==  0) *type = AARS_TYPE_RELATIVE;
    else if(compare_texts(text, "sequence") ==  0) *type = AARS_TYPE_SEQUENCE;
    else return  MKSBMLI_INVALID_PARAMETER;

    return MKSBMLI_NO_ERROR;
}

int get_aars_type_text(AARS_TYPE type, char* text) {
    switch(type) {
    case AARS_TYPE_AIM: copy_text(text, "aim"); break;
    case AARS_TYPE_ABSOLUTE: copy_text(text, "absolute"); break;
    case AARS_TYPE_RELATIVE: copy_text(text, "relative"); break;
    case AARS_TYPE_SEQUENCE: copy_text(text, "sequence"); break;
    default: return MKSBMLI_INVALID_PARAMETER;
    }

    return MKSBMLI_NO_ERROR;
}

int get_ars_type(const char* text, ARS_TYPE* type) {
    if(compare_texts(text, "absolute") ==  0) *type = ARS_TYPE_ABSOLUTE;
    else if(compare_texts(text, "relative") ==  0) *type = ARS_TYPE_RELATIVE;
    else if(compare_texts(text, "sequence") ==  0) *type = ARS_TYPE_SEQUENCE;
    else return  MKSBMLI_INVALID_PARAMETER;

    return MKSBMLI_NO_ERROR;
}

int get_ars_type_text(ARS_TYPE type, char* text) {
    switch(type) {
    case ARS_TYPE_ABSOLUTE: copy_text(text, "absolute"); break;
    case ARS_TYPE_RELATIVE: copy_text(text, "relative"); break;
    case ARS_TYPE_SEQUENCE: copy_text(text, "sequence"); break;
    default: return MKSBMLI_INVALID_PARAMETER;
    }

    return MKSBMLI_NO_ERROR;
}

float get_random_unit_float() {
    return (float)(float)GET_RANDOM() / (float)RAND_MAX;
}

float calc_angle_degrees(BULLETML_ATTRIBUTE_TYPE type, Vector2D source, Vector2D target) {
    Vector2D unit_reference_vector = (Vector2D){1.0f, 0.0f};
    if(type == BULLETML_ATTRIBUTE_TYPE_VERTICAL) {
        unit_reference_vector = (Vector2D){0.0f, 1.0f};
    }
    Vector2D unit_direction_vector = Vector2DNormalize(Vector2DSubtract(target, source));

    float radian_result = acosf(Vector2DDotProduct(unit_reference_vector, unit_direction_vector));
    float degree_result = 0.0f;
    if((type == BULLETML_ATTRIBUTE_TYPE_VERTICAL && unit_direction_vector.x >= 0)
        || (type == BULLETML_ATTRIBUTE_TYPE_HORIZONTAL && unit_direction_vector.y <= 0)) degree_result = -DEGREES_FACTOR * radian_result;
    else degree_result = DEGREES_FACTOR * radian_result;

    return degree_result;
}

bool string_contained(char* source, char* text) {
    return strstr(source, text) != NULL;
}

void replace_keyword(char* target_text, char *source_text, const char *old_text, const char *new_text) {
    char *pos;

    pos = strstr(source_text, old_text);
    if (pos == NULL) return;

    int index = pos - source_text;
    strncpy(target_text, source_text, index);
    target_text[index] = '\0';

    strcat(target_text, new_text);

    strcat(target_text, pos + strlen(old_text));
}

void replace_rand_keyword(char target[MKSBMLI_MAX_TEXT_LENGTH], char source[MKSBMLI_MAX_TEXT_LENGTH]) {
    char rand_keyword[] = "$rand";

    if(!string_contained(source, rand_keyword)) {
        strncpy(target, source, MKSBMLI_MAX_TEXT_LENGTH);
        return;
    }

    char random_value_text[MKSBMLI_MAX_NUMBER_TEXT_LENGTH];
    memset(random_value_text, 0, MKSBMLI_MAX_NUMBER_TEXT_LENGTH);
    snprintf(random_value_text, MKSBMLI_MAX_NUMBER_TEXT_LENGTH, "%f", get_random_unit_float());

    replace_keyword(target, source, rand_keyword, random_value_text);
}

void replace_rank_keyword(float rank, char target[MKSBMLI_MAX_TEXT_LENGTH], char source[MKSBMLI_MAX_TEXT_LENGTH]) {
    char rand_keyword[] = "$rank";

    if(!string_contained(source, rand_keyword)) {
        strncpy(target, source, MKSBMLI_MAX_TEXT_LENGTH);
        return;
    }

    char rank_value_text[MKSBMLI_MAX_NUMBER_TEXT_LENGTH];
    memset(rank_value_text, 0, MKSBMLI_MAX_NUMBER_TEXT_LENGTH);
    snprintf(rank_value_text, MKSBMLI_MAX_NUMBER_TEXT_LENGTH, "%f", rank);

    replace_keyword(target, source, rand_keyword, rank_value_text);
}

void replace_parameters_keyword(float* params, int nos_params, char target[MKSBMLI_MAX_TEXT_LENGTH], char source[MKSBMLI_MAX_TEXT_LENGTH]) {
    if(params == NULL || nos_params <= 0) {
        copy_text(target, source);
        return;
    }

    for(int index = 0; index < nos_params; index++) {
        char param_keyword[MKSBMLI_MAX_NUMBER_TEXT_LENGTH];
        memset(param_keyword, 0, MKSBMLI_MAX_NUMBER_TEXT_LENGTH);
        snprintf(param_keyword, MKSBMLI_MAX_NUMBER_TEXT_LENGTH, "$%d", index + 1);

        if(!string_contained(source, param_keyword)) {
            strncpy(target, source, MKSBMLI_MAX_TEXT_LENGTH);
            continue;
        }

        char param_value_text[MKSBMLI_MAX_NUMBER_TEXT_LENGTH];
        memset(param_value_text, 0, MKSBMLI_MAX_NUMBER_TEXT_LENGTH);
        snprintf(param_value_text, MKSBMLI_MAX_NUMBER_TEXT_LENGTH, "%f", params[index]);

        replace_keyword(target, source, param_keyword, param_value_text);
    }
}
