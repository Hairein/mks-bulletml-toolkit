#include "utility.h"

int extract_xml_property_text(xmlNode* node, char* name, char* text) {
    xmlChar *xml_text = xmlGetProp(node, (const xmlChar *)name);
    if (xml_text) {
        strncpy(text, (const char*)xml_text, MKSBMLI_MAX_TEXT_LENGTH - 1);
        xmlFree(xml_text);

        return MKSBMLI_NO_ERROR;
    }

    return MKSBMLI_XML_PARSE_ERROR;
}

void extract_xml_property_label_text(xmlNode* node, char* text) {
    strncpy(text, "", MKSBMLI_MAX_TEXT_LENGTH - 1);

    char entry[MKSBMLI_MAX_TEXT_LENGTH];
    if(extract_xml_property_text(node, "label", entry) == MKSBMLI_NO_ERROR) {
        strncpy(entry, text, MKSBMLI_MAX_TEXT_LENGTH - 1);
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

int copy_text(char* target, const char* source) {
    strncpy(target, source, MKSBMLI_MAX_TEXT_LENGTH);
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
