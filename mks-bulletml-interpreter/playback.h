#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "mks-bulletml-interpreter.h"
#include "virtual_bullet.h"
#include "bulletml.h"
#include "action.h"

typedef struct {
    MKSBMLI_PLAYBACK_HANDLE handle;
    bool is_playing;
    char xml_filename[MKSBMLI_XML_FILENAME_MAX_LENGTH];

    MKSBMLI_BULLET_HANDLE next_free_bullet_handle;
    VirtualBullet bullets[MKSBMLI_MAX_BULLETS];

    unsigned int next_free_base_index;
    BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS];
} Playback;

int init_playback(Playback* playback, const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE handle);
void shutdown_playback(Playback* playback);

void update_playback(Playback* playback);
void reset_playback(Playback* playback);
void set_playing(Playback* playback, bool flag);

int get_bullets(Playback* playback, size_t max_bullets, VirtualBullet* bullets, size_t* nos_bullets);
int destroy_bullets(Playback* playback, size_t nos_bullet_handles, MKSBMLI_BULLET_HANDLE* handles);

int parse_xml_file(Playback* playback, const char* xml_filename);
void traverse_xml_file(Playback* playback, xmlNode* node, BulletmlBase* parent);

void parse_bulletml(Playback* playback, xmlNode* node, Bulletml* bulletml);
void parse_action(Playback* playback, xmlNode* node, BulletmlBase* parent, Action* action);

#endif // PLAYBACK_H
