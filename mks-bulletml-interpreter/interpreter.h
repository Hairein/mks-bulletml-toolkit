#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#include "bulletml-base.h"
#include "virtual_bullet_manager.h"

typedef struct {
    BULLETML_ATTRIBUTE_TYPE bulletml_attribute;
    Vector2 player_position;

    bool can_play_frame;
    unsigned int top_action_index;
    bool play_frame_finished;

    bool is_waiting;
    int waiting_action_index;
    int waiting_index;
    unsigned int wait_frames;

    VirtualBulletManager* vbm;
} Interpreter;

void init_interpreter(Interpreter* interpreter, VirtualBulletManager* vbm);

void reset_playhead(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_frame(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

bool find_top_action_index(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_action(Interpreter* interpreter, int parent_action_index, int index, unsigned int offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_repeat(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_action_ref(Interpreter* interpreter, int parent_action_index, int index, unsigned int offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_fire(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_fire_ref(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_wait(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_change_speed(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_change_direction(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_accel(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_vanish(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

int get_times_value(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
int get_term_value(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
float get_direction(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], AARS_TYPE* direction_type);
float get_speed(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* speed_type);
float get_horizontal(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* horizontal_type);
float get_vertical(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* vertical_type);

bool find_child_element_of_type(Interpreter* interpreter, int parent_index, BULLETML_ELEMENT_TYPE type, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);
bool find_element_by_label(Interpreter* interpreter, BULLETML_ELEMENT_TYPE type, char* label, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);

#endif // INTERPRETER_H
