#ifndef UTILITY_H
#define UTILITY_H

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "mksbmli-defines.h"

int extract_xml_property_text(xmlNode* node, char*name, char* text);

void extract_xml_property_label_text(xmlNode* node, char* text);
void extract_xml_property_bulletml_type(xmlNode* node, BULLETML_ATTRIBUTE_TYPE* type);
void extract_xml_property_aars_type(xmlNode* node, AARS_TYPE* type);
void extract_xml_property_ars_type(xmlNode* node, ARS_TYPE* type);
void extract_xml_text_content(xmlNode* node, char content[MKSBMLI_MAX_TEXT_LENGTH], const char* default_text);

void copy_filename(char* target, const char* source);
void copy_text(char* target, const char* source);
int compare_texts(const char* text1, const char* text2);

int get_bulletml_attribute_type(const char* text, BULLETML_ATTRIBUTE_TYPE* type);
int get_bulletml_attribute_type_text(BULLETML_ATTRIBUTE_TYPE type, char* text);

int get_aars_type(const char* text, AARS_TYPE* type);
int get_aars_type_text(AARS_TYPE type, char* text);

int get_ars_type(const char* text, ARS_TYPE* type);
int get_ars_type_text(ARS_TYPE type, char* text);

float get_random_unit_float();

#endif // UTILITY_H
