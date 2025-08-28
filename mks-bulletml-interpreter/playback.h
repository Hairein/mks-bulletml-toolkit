#ifndef PLAYBACK_H
#define PLAYBACK_H

#include "mks-bulletml-interpreter.h"
#include "virtual_bullet.h"
#include "bulletml.h"
#include "bullet.h"
#include "action.h"
#include "fire.h"
#include "change-direction.h"
#include "change-speed.h"
#include "accel.h"
#include "wait.h"
#include "vanish.h"
#include "repeat.h"
#include "direction.h"
#include "speed.h"
#include "horizontal.h"
#include "vertical.h"
#include "term.h"
#include "times.h"
#include "bullet-ref.h"
#include "action-ref.h"
#include "fire-ref.h"
#include "param.h"

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
void parse_bullet(Playback* playback, xmlNode* node, BulletmlBase* parent, Bullet* bullet);
void parse_action(Playback* playback, xmlNode* node, BulletmlBase* parent, Action* action);
void parse_fire(Playback* playback, xmlNode* node, BulletmlBase* parent, Fire* fire);
void parse_change_direction(Playback* playback, xmlNode* node, BulletmlBase* parent, ChangeDirection* change_direction);
void parse_change_speed(Playback* playback, xmlNode* node, BulletmlBase* parent, ChangeSpeed* change_speed);
void parse_accel(Playback* playback, xmlNode* node, BulletmlBase* parent, Accel* accel);
void parse_wait(Playback* playback, xmlNode* node, BulletmlBase* parent, Wait* wait);
void parse_vanish(Playback* playback, xmlNode* node, BulletmlBase* parent, Vanish* vanish);
void parse_repeat(Playback* playback, xmlNode* node, BulletmlBase* parent, Repeat* repeat);
void parse_direction(Playback* playback, xmlNode* node, BulletmlBase* parent, Direction* direction);
void parse_speed(Playback* playback, xmlNode* node, BulletmlBase* parent, Speed* speed);

#endif // PLAYBACK_H
