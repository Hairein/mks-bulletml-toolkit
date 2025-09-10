#ifndef UTILITY_H
#define UTILITY_H

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "mksbmli-defines.h"
#include "Vector2D.h"

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

float calc_angle_degrees(BULLETML_ATTRIBUTE_TYPE type, Vector2D source, Vector2D target);

bool string_contained(char* source, char* text);
void replace_keyword(char*target_text, char *source_text, const char *old_text, const char *new_text);
void replace_rand_keyword(char target[MKSBMLI_MAX_TEXT_LENGTH], char source[MKSBMLI_MAX_TEXT_LENGTH]);
void replace_rank_keyword(float rank, char target[MKSBMLI_MAX_TEXT_LENGTH], char source[MKSBMLI_MAX_TEXT_LENGTH]);
void replace_parameters_keyword(float* params, int nos_params, char target[MKSBMLI_MAX_TEXT_LENGTH], char source[MKSBMLI_MAX_TEXT_LENGTH]);

#endif // UTILITY_H
