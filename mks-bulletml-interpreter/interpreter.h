#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#include "bulletml-base.h"
#include "action_info_block.h"
#include "virtual_bullet_manager.h"

typedef struct {
    BULLETML_ATTRIBUTE_TYPE bulletml_attribute;
    Vector2D emitter_position;
    Vector2D player_position;
    float rank;

    VirtualBulletManager* vbm;
    unsigned int next_free_bullet_id;

    bool first_frame;

    ActionInfoBlock active_actions[MKSBMLI_MAX_ELEMENTS];
    // unsigned int next_free_action_id;

    bool is_valid;
} Interpreter;

void init_interpreter(Interpreter* interpreter, VirtualBulletManager* vbm);

void reset_playhead(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_frame(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_action(Interpreter* interpreter, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_wait(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_fire_ref(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_fire(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_repeat(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_change_speed(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_change_direction(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_accel(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_vanish(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

int find_top_action_index(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
int insert_action_for_playback(Interpreter* interpreter, int parent_action_index, int action_index, unsigned int offset, int parent_block_index, float* params, int nos_params, unsigned int bullet_id);
bool check_actions_playback_finished(Interpreter* interpreter);

float get_direction(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], AARS_TYPE* direction_type);
float get_speed(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* speed_type);
int get_times_value(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
int get_term_value(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
float get_horizontal(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* horizontal_type);
float get_vertical(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* vertical_type);

bool find_element_by_label(Interpreter* interpreter, BULLETML_ELEMENT_TYPE type, char* label, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);
bool find_child_element_of_type(Interpreter* interpreter, int parent_element_index, BULLETML_ELEMENT_TYPE type, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);
void get_params_for_element(Interpreter* interpreter, int parent_element_index, float* action_params, int nos_action_params, int max_params, float* params, int* nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

int get_ancestor_bullet_id(ActionInfoBlock* block, ActionInfoBlock* blocks);
int get_current_or_ancestor_bullet_id(ActionInfoBlock* block, ActionInfoBlock* blocks);

#endif // INTERPRETER_H
