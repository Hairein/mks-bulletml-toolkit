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

int copy_text(char* target, const char* source) {
    strncpy(target, source, MKSBMLI_MAX_TEXT_LENGTH);
}

int compare_texts(const char* text1, const char* text2) {
    return strncmp(text1, text2, MKSBMLI_MAX_TEXT_LENGTH);
}
