#ifndef PLAYBACK_H
#define PLAYBACK_H

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "mksbmli-defines.h"
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
#include "interpreter.h"
#include "virtual_bullet_manager.h"

typedef struct {
    MKSBMLI_PLAYBACK_HANDLE handle;
    bool is_playing;
    char xml_filename[MKSBMLI_XML_FILENAME_MAX_LENGTH];

    unsigned int next_free_base_index;
    BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS];

    Interpreter interpreter;
    VirtualBulletManager virtual_bullet_manager;
} Playback;

int init_playback(Playback* playback, const char* xml_filename, MKSBMLI_PLAYBACK_HANDLE handle);
void shutdown_playback(Playback* playback);

void update_playback(Playback* playback);
void reset_playback(Playback* playback);
void set_playing(Playback* playback, bool flag);

void get_bullets(Playback* playback, int max_bullets, VirtualBullet** bullets, int* nos_bullets);
void destroy_bullets(Playback* playback, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles);
void clear_bullets(Playback* playback);

void set_emitter_position(Playback* playback, Vector2D emitter_position);
void set_player_position(Playback* playback, Vector2D player_position);
void set_rank(Playback* playback, float rank);

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
void parse_horizontal(Playback* playback, xmlNode* node, BulletmlBase* parent, Horizontal* horizontal);
void parse_vertical(Playback* playback, xmlNode* node, BulletmlBase* parent, Vertical* vertical);
void parse_term(Playback* playback, xmlNode* node, BulletmlBase* parent, Term* term);
void parse_times(Playback* playback, xmlNode* node, BulletmlBase* parent, Times* times);
void parse_bullet_ref(Playback* playback, xmlNode* node, BulletmlBase* parent, BulletRef* bullet_ref);
void parse_action_ref(Playback* playback, xmlNode* node, BulletmlBase* parent, ActionRef* action_ref);
void parse_fire_ref(Playback* playback, xmlNode* node, BulletmlBase* parent, FireRef* fire_ref);
void parse_param(Playback* playback, xmlNode* node, BulletmlBase* parent, Param* param);

#endif // PLAYBACK_H
