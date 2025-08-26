#include "playback.h"

int init_playback(Playback* playback, const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE handle) {
    if(handle == 0) return MKSBMLI_NO_ERROR;

    size_t filename_length = strlen(xml_filename);
    if(filename_length == 0 || filename_length >= MKSBMLI_XML_FILENAME_MAX_LENGTH) return MKSBMLI_INVALID_XML_FILENAME;

    memset((void*) playback, 0, sizeof(Playback));

    playback->handle = handle;
    playback->is_playing = false;
    strncpy((void*) playback->xml_filename, xml_filename, filename_length);

    playback->next_free_bullet_handle = 1;
    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        playback->bullets[index].handle = 0;
    }

    playback->next_free_base_index = 0;
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        playback->bulletml_bases[index] = NULL;
    }

    parse_xml_file(playback, xml_filename);

    return MKSBMLI_NO_ERROR;
}

void shutdown_playback(Playback* playback) {
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        if (playback->bulletml_bases[index] != NULL) {
            // TODO free depends on type of element
            //free(playback->bulletml_bases[index]);
            playback->bulletml_bases[index] = NULL;
        }
    }
}

void update_playback(Playback* playback) {
    // TODO
}

void reset_playback(Playback* playback) {
    // TODO
}

void set_playing(Playback* playback, bool flag) {
    playback->is_playing = flag;
}

int get_bullets(Playback* playback, size_t max_bullets, VirtualBullet* bullets, size_t* nos_bullets)
{
    return MKSBMLI_NO_ERROR;
}

int destroy_bullets(Playback* playback, size_t nos_bullet_handles, MKSBMLI_BULLET_HANDLE* handles) {
    return MKSBMLI_NO_ERROR;
}

int parse_xml_file(Playback* playback, const char* xml_filename) {
    xmlDocPtr doc = xmlReadFile(xml_filename, NULL, 0);
    if (doc == NULL) {
        fprintf(stderr, "Error loading XML file: %s\n", xml_filename);
        return MKSBMLI_XML_FILE_LOAD_ERROR;
    }

    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (root == NULL) {
        fprintf(stderr, "Empty XML file\n");
        xmlFreeDoc(doc);
        return MKSBMLI_XML_FILE_LOAD_ERROR;
    }

    if(strcmp((const char*)root->name, "bulletml") != 0) {
        fprintf(stderr, "Unknown XML file contents\n");
        xmlFreeDoc(doc);
        return MKSBMLI_XML_FILE_LOAD_ERROR;
    }

    traverse_xml_file(playback, root, NULL);

    xmlFreeDoc(doc);

    return MKSBMLI_NO_ERROR;
}

void traverse_xml_file(Playback* playback, xmlNode *node, BulletmlBase* parent) {
    BulletmlBase* new_parent = NULL;

    for (xmlNode *cur = node; cur != NULL; cur = cur->next) {
        if (cur->type == XML_ELEMENT_NODE) {
            printf("Element: %s, Parent: %s\n",
                   (const char *)cur->name, (const char *)cur->parent->name);

            if (xmlStrcmp(cur->name, (const xmlChar *)"bulletml") == 0) {
                Bulletml* new_bulletml = calloc(sizeof(Bulletml), 1);
                parse_bulletml(playback, node, new_bulletml);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_bulletml;
                new_parent = (BulletmlBase*)new_bulletml;
                dump_bulletml(new_bulletml);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"bullet") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"action") == 0) {
                Action* new_action = calloc(sizeof(Action), 1);
                parse_action(playback, node, parent, new_action);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_action;
                new_parent = (BulletmlBase*)new_action;
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"fire") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"changeDirection") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"changeSpeed") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"accel") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"wait") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"vanish") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"repeat") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"direction") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"speed") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"horizontal") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"vertical") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"term") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"times") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"bulletRef") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"actionRef") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"fireRef") == 0) {
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"param") == 0) {
            }
        }

        traverse_xml_file(playback, cur->children, new_parent);
    }
}

void parse_bulletml(Playback* playback, xmlNode* node, Bulletml* bulletml) {
    init_bulletml(bulletml, NULL, BULLETML_ATTRIBUTE_TYPE_NONE);

    xmlChar *type = xmlGetProp(node, (const xmlChar *)"type");
    if (type) {
        if (xmlStrcmp(type, (const xmlChar *)"vertical") == 0) {
            bulletml->attribute = BULLETML_ATTRIBUTE_TYPE_VERTICAL;
        } else if (xmlStrcmp(type, (const xmlChar *)"horizontal") == 0) {
            bulletml->attribute = BULLETML_ATTRIBUTE_TYPE_HORIZONTAL;
        }

        xmlFree(type);
    }
}

void parse_action(Playback* playback, xmlNode* node, BulletmlBase* parent, Action* action) {
    init_action(action, parent, "xxx");
}
