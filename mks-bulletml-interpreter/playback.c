#include <string.h>

#include "utility.h"
#include "playback.h"

int init_playback(Playback* playback, const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE handle) {
    if(handle == 0) return MKSBMLI_NO_ERROR;

    size_t filename_length = strlen(xml_filename);
    if(filename_length == 0 || filename_length >= MKSBMLI_XML_FILENAME_MAX_LENGTH) return MKSBMLI_INVALID_XML_FILENAME;

    memset(playback, 0, sizeof(*playback));

    playback->handle = handle;
    playback->is_playing = false;
    copy_text(playback->xml_filename, xml_filename);

    init_virtual_bullet_manager(&playback->virtual_bullet_manager);
    init_interpreter(&playback->interpreter, &playback->virtual_bullet_manager);

    playback->next_free_base_index = 0;
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        playback->bulletml_bases[index] = NULL;
    }

    parse_xml_file(playback, xml_filename);

    reset_playback(playback);

    return MKSBMLI_NO_ERROR;
}

void shutdown_playback(Playback* playback) {
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        if (playback->bulletml_bases[index] != NULL) {
            if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_BULLETML) free((Bulletml*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_BULLET) free((Bullet*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_ACTION) free((Action*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_FIRE) free((Fire*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_CHANGE_DIRECTION) free((ChangeDirection*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_CHANGE_SPEED) free((ChangeSpeed*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_ACCEL) free((Accel*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_WAIT) free((Wait*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_VANISH) free((Vanish*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_REPEAT) free((Repeat*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_DIRECTION) free((Direction*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_SPEED) free((Speed*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_HORIZONTAL) free((Horizontal*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_VERTICAL) free((Vertical*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_TERM) free((Term*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_TIMES) free((Times*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_BULLET_REF) free((BulletRef*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_ACTION_REF) free((ActionRef*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_FIRE_REF) free((FireRef*)playback->bulletml_bases[index]);
            else if(playback->bulletml_bases[index]->type == BULLETML_ELEMENT_TYPE_PARAM) free((Param*)playback->bulletml_bases[index]);

            playback->bulletml_bases[index] = NULL;
        }
    }
}

void update_playback(Playback* playback) {
    if(!playback->is_playing) return;

    update_virtual_bullet_manager(&playback->virtual_bullet_manager);

    play_frame(&playback->interpreter, playback->bulletml_bases);
}

void reset_playback(Playback* playback) {
    reset_playhead(&playback->interpreter, playback->bulletml_bases);
}

void set_playing(Playback* playback, bool flag) {
    playback->is_playing = flag;

    reset_playback(playback);
}

void get_bullets(Playback* playback, int max_bullets, VirtualBullet** bullets, int* nos_bullets)
{
    get_vbm_bullets(&playback->virtual_bullet_manager, max_bullets, bullets, nos_bullets);
}

void destroy_bullets(Playback* playback, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles) {
    destroy_vbm_bullets(&playback->virtual_bullet_manager, bullet_handles, nos_bullet_handles);
}

void clear_bullets(Playback* playback) {
    clear_vbm_bullets(&playback->virtual_bullet_manager);
}

void set_emitter_position(Playback* playback, Vector2D emitter_position) {
    playback->interpreter.emitter_position = emitter_position;
}

void set_player_position(Playback* playback, Vector2D player_position) {
    playback->interpreter.player_position = player_position;
}

void set_rank(Playback* playback, float rank) {
    if(rank < 0.0f) playback->interpreter.rank = 0.0f;
    else if(rank > 1.0f) playback->interpreter.rank = 1.0f;
    else playback->interpreter.rank = rank;
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

    printf("+++++++++\n");
    traverse_xml_file(playback, root, NULL);
    printf("---------\n");

    xmlFreeDoc(doc);

    return MKSBMLI_NO_ERROR;
}

void traverse_xml_file(Playback* playback, xmlNode *node, BulletmlBase* parent) {
    BulletmlBase* new_parent = NULL;

    int index = 0;
    for (xmlNode *cur = node; cur != NULL; cur = cur->next) {
        if (cur->type == XML_ELEMENT_NODE) {
            // printf("%d) Element: %s, Parent: %s\n",
            //        index++,
            //        (const char *)cur->name, (const char *)cur->parent->name);

            if (xmlStrcmp(cur->name, (const xmlChar *)"bulletml") == 0) {
                Bulletml* new_bulletml = calloc(sizeof(Bulletml), 1);
                parse_bulletml(playback, cur, new_bulletml);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_bulletml;
                new_parent = (BulletmlBase*)new_bulletml;
                dump_bulletml(new_bulletml);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"bullet") == 0) {
                Bullet* new_bullet = calloc(sizeof(Bullet), 1);
                parse_bullet(playback, cur, parent, new_bullet);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_bullet;
                new_parent = (BulletmlBase*)new_bullet;
                dump_bullet(new_bullet);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"action") == 0) {
                Action* new_action = calloc(sizeof(Action), 1);
                parse_action(playback, cur, parent, new_action);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_action;
                new_parent = (BulletmlBase*)new_action;
                dump_action(new_action);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"fire") == 0) {
                Fire* new_fire = calloc(sizeof(Fire), 1);
                parse_fire(playback, cur, parent, new_fire);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_fire;
                new_parent = (BulletmlBase*)new_fire;
                dump_fire(new_fire);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"changeDirection") == 0) {
                ChangeDirection* new_change_direction = calloc(sizeof(ChangeDirection), 1);
                parse_change_direction(playback, cur, parent, new_change_direction);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_change_direction;
                new_parent = (BulletmlBase*)new_change_direction;
                dump_change_direction(new_change_direction);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"changeSpeed") == 0) {
                ChangeSpeed* new_change_speed = calloc(sizeof(ChangeSpeed), 1);
                parse_change_speed(playback, cur, parent, new_change_speed);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_change_speed;
                new_parent = (BulletmlBase*)new_change_speed;
                dump_change_speed(new_change_speed);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"accel") == 0) {
                Accel* new_accel = calloc(sizeof(Accel), 1);
                parse_accel(playback, cur, parent, new_accel);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_accel;
                new_parent = (BulletmlBase*)new_accel;
                dump_accel(new_accel);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"wait") == 0) {
                Wait* new_wait = calloc(sizeof(Wait), 1);
                parse_wait(playback, cur, parent, new_wait);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_wait;
                new_parent = (BulletmlBase*)new_wait;
                dump_wait(new_wait);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"vanish") == 0) {
                Vanish* new_vanish = calloc(sizeof(Vanish), 1);
                parse_vanish(playback, cur, parent, new_vanish);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_vanish;
                new_parent = (BulletmlBase*)new_vanish;
                dump_vanish(new_vanish);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"repeat") == 0) {
                Repeat* new_repeat = calloc(sizeof(Repeat), 1);
                parse_repeat(playback, cur, parent, new_repeat);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_repeat;
                new_parent = (BulletmlBase*)new_repeat;
                dump_repeat(new_repeat);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"direction") == 0) {
                Direction* new_direction = calloc(sizeof(Direction), 1);
                parse_direction(playback, cur, parent, new_direction);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_direction;
                new_parent = (BulletmlBase*)new_direction;
                dump_direction(new_direction);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"speed") == 0) {
                Speed* new_speed = calloc(sizeof(Speed), 1);
                parse_speed(playback, cur, parent, new_speed);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_speed;
                new_parent = (BulletmlBase*)new_speed;
                dump_speed(new_speed);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"horizontal") == 0) {
                Horizontal* new_horizontal = calloc(sizeof(Horizontal), 1);
                parse_horizontal(playback, cur, parent, new_horizontal);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_horizontal;
                new_parent = (BulletmlBase*)new_horizontal;
                dump_horizontal(new_horizontal);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"vertical") == 0) {
                Vertical* new_vertical = calloc(sizeof(Vertical), 1);
                parse_vertical(playback, cur, parent, new_vertical);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_vertical;
                new_parent = (BulletmlBase*)new_vertical;
                dump_vertical(new_vertical);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"term") == 0) {
                Term* new_term = calloc(sizeof(Term), 1);
                parse_term(playback, cur, parent, new_term);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_term;
                new_parent = (BulletmlBase*)new_term;
                dump_term(new_term);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"times") == 0) {
                Times* new_times = calloc(sizeof(Times), 1);
                parse_times(playback, cur, parent, new_times);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_times;
                new_parent = (BulletmlBase*)new_times;
                dump_times(new_times);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"bulletRef") == 0) {
                BulletRef* new_bullet_ref = calloc(sizeof(BulletRef), 1);
                parse_bullet_ref(playback, cur, parent, new_bullet_ref);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_bullet_ref;
                new_parent = (BulletmlBase*)new_bullet_ref;
                dump_bullet_ref(new_bullet_ref);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"actionRef") == 0) {
                ActionRef* new_action_ref = calloc(sizeof(ActionRef), 1);
                parse_action_ref(playback, cur, parent, new_action_ref);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_action_ref;
                new_parent = (BulletmlBase*)new_action_ref;
                dump_action_ref(new_action_ref);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"fireRef") == 0) {
                FireRef* new_fire_ref = calloc(sizeof(FireRef), 1);
                parse_fire_ref(playback, cur, parent, new_fire_ref);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_fire_ref;
                new_parent = (BulletmlBase*)new_fire_ref;
                dump_fire_ref(new_fire_ref);
            } else if (xmlStrcmp(cur->name, (const xmlChar *)"param") == 0) {
                Param* new_param = calloc(sizeof(Param), 1);
                parse_param(playback, cur, parent, new_param);
                playback->bulletml_bases[playback->next_free_base_index++] = (BulletmlBase*)new_param;
                new_parent = (BulletmlBase*)new_param;
                dump_param(new_param);
            }
        }

        traverse_xml_file(playback, cur->children, new_parent);
    }
}

void parse_bulletml(Playback* playback, xmlNode* node, Bulletml* bulletml) {
    BULLETML_ATTRIBUTE_TYPE type;
    extract_xml_property_bulletml_type(node, &type);
    init_bulletml(bulletml, NULL, type);
}

void parse_bullet(Playback* playback, xmlNode* node, BulletmlBase* parent, Bullet* bullet) {
    char label[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_property_label_text(node, label);
    init_bullet(bullet, parent, label);
}

void parse_action(Playback* playback, xmlNode* node, BulletmlBase* parent, Action* action) {
    char label[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_property_label_text(node, label);
    init_action(action, parent, label);
}

void parse_fire(Playback* playback, xmlNode* node, BulletmlBase* parent, Fire* fire) {
    char label[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_property_label_text(node, label);
    init_fire(fire, parent, label);
}

void parse_change_direction(Playback* playback, xmlNode* node, BulletmlBase* parent, ChangeDirection* change_direction) {
    init_change_direction(change_direction, parent);
}

void parse_change_speed(Playback* playback, xmlNode* node, BulletmlBase* parent, ChangeSpeed* change_speed) {
    init_change_speed(change_speed, parent);
}

void parse_accel(Playback* playback, xmlNode* node, BulletmlBase* parent, Accel* accel) {
    init_accel(accel, parent);
}

void parse_wait(Playback* playback, xmlNode* node, BulletmlBase* parent, Wait* wait) {
    char frames[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, frames, "1");

    init_wait(wait, parent, frames);
}

void parse_vanish(Playback* playback, xmlNode* node, BulletmlBase* parent, Vanish* vanish) {
    init_vanish(vanish, parent);
}

void parse_repeat(Playback* playback, xmlNode* node, BulletmlBase* parent, Repeat* repeat) {
    init_repeat(repeat, parent);
}

void parse_direction(Playback* playback, xmlNode* node, BulletmlBase* parent, Direction* direction) {
    AARS_TYPE type = AARS_TYPE_ABSOLUTE;
    extract_xml_property_aars_type(node, &type);

    char degrees[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, degrees, "0.0");

    init_direction(direction, parent, type, degrees);
}

void parse_speed(Playback* playback, xmlNode* node, BulletmlBase* parent, Speed* speed) {
    ARS_TYPE type = ARS_TYPE_ABSOLUTE;
    extract_xml_property_ars_type(node, &type);

    char velocity[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, velocity, "0.0");

    init_speed(speed, parent, type, velocity);
}

void parse_horizontal(Playback* playback, xmlNode* node, BulletmlBase* parent, Horizontal* horizontal) {
    ARS_TYPE type = ARS_TYPE_ABSOLUTE;
    extract_xml_property_ars_type(node, &type);

    char acceleration[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, acceleration, "0.0");

    init_horizontal(horizontal, parent, type, acceleration);
}

void parse_vertical(Playback* playback, xmlNode* node, BulletmlBase* parent, Vertical* vertical) {
    ARS_TYPE type = ARS_TYPE_ABSOLUTE;
    extract_xml_property_ars_type(node, &type);

    char acceleration[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, acceleration, "0.0");

    init_vertical(vertical, parent, type, acceleration);
}

void parse_term(Playback* playback, xmlNode* node, BulletmlBase* parent, Term* term) {
    ARS_TYPE type = ARS_TYPE_ABSOLUTE;
    extract_xml_property_ars_type(node, &type);

    char frames[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, frames, "1");

    init_term(term, parent, frames);
}

void parse_times(Playback* playback, xmlNode* node, BulletmlBase* parent, Times* times) {
    ARS_TYPE type = ARS_TYPE_ABSOLUTE;
    extract_xml_property_ars_type(node, &type);

    char number[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, number, "1");

    init_times(times, parent, number);
}

void parse_bullet_ref(Playback* playback, xmlNode* node, BulletmlBase* parent, BulletRef* bullet_ref) {
    char label[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_property_label_text(node, label);
    init_bullet_ref(bullet_ref, parent, label);
}

void parse_action_ref(Playback* playback, xmlNode* node, BulletmlBase* parent, ActionRef* action_ref) {
    char label[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_property_label_text(node, label);
    init_action_ref(action_ref, parent, label);
}

void parse_fire_ref(Playback* playback, xmlNode* node, BulletmlBase* parent, FireRef* fire_ref) {
    char label[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_property_label_text(node, label);
    init_fire_ref(fire_ref, parent, label);
}

void parse_param(Playback* playback, xmlNode* node, BulletmlBase* parent, Param* param) {
    char contents[MKSBMLI_MAX_TEXT_LENGTH];
    extract_xml_text_content(node, contents, "1.0");

    init_param(param, parent, contents);
}

