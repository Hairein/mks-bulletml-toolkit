#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#include "bulletml-base.h"
#include "action_info_block.h"
#include "virtual_bullet_manager.h"

typedef struct {
    BULLETML_ATTRIBUTE_TYPE bulletml_attribute;
    Vector2 player_position;

    VirtualBulletManager* vbm;
    unsigned int next_free_bullet_id;

    bool first_frame;

    ActionInfoBlock active_actions[MKSBMLI_MAX_ELEMENTS];
    // unsigned int next_free_action_id;
} Interpreter;

void init_interpreter(Interpreter* interpreter, VirtualBulletManager* vbm);

void reset_playhead(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_frame(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_action(Interpreter* interpreter, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_wait(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_fire_ref(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_fire(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_repeat(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

int find_top_action_index(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void insert_action_for_playback(Interpreter* interpreter, int parent_action_index, int action_index, unsigned int offset, int parent_block_index);
bool check_actions_playback_finished(Interpreter* interpreter);

float get_direction(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], AARS_TYPE* direction_type);
float get_speed(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* speed_type);
int get_times_value(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

bool find_element_by_label(Interpreter* interpreter, BULLETML_ELEMENT_TYPE type, char* label, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);
bool find_child_element_of_type(Interpreter* interpreter, int parent_element_index, BULLETML_ELEMENT_TYPE type, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);

int get_ancestor_bullet_id(ActionInfoBlock* block, ActionInfoBlock* blocks);

// void insert_action_for_playback(Interpreter* interpreter, int parent_action_index, int action_index);
// void remove_action_from_playback(Interpreter* interpreter, int action_index);
// bool check_actions_playback_finished(Interpreter* interpreter);

// bool find_top_action_index(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_action(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, unsigned int offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_repeat(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_action_ref(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, unsigned int offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_fire(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_fire_ref(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_wait(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_change_speed(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_change_direction(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_accel(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// void play_vanish(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

// int get_times_value(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// int get_term_value(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
// float get_direction(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], AARS_TYPE* direction_type);
// float get_speed(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* speed_type);
// float get_horizontal(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* horizontal_type);
// float get_vertical(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* vertical_type);

// bool find_child_element_of_type(Interpreter* interpreter, int parent_element_index, BULLETML_ELEMENT_TYPE type, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);
// bool find_element_by_label(Interpreter* interpreter, BULLETML_ELEMENT_TYPE type, char* label, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);

#endif // INTERPRETER_H
