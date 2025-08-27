#ifndef UTILITY_H
#define UTILITY_H

#include "mks-bulletml-interpreter.h"

int extract_xml_property_text(xmlNode* node, char*name, char* text);

int copy_text(char* target, const char* source);
int compare_texts(const char* text1, const char* text2);

#endif // UTILITY_H
