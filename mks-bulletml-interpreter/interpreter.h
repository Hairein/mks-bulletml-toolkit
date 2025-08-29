#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#include "bulletml-base.h"

typedef struct {
    BULLETML_ATTRIBUTE_TYPE bulletml_attribute;

    bool can_play_frame;
    unsigned int top_action_index;
    bool play_frame_finished;
} Interpreter;

void reset_playhead(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_frame(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

bool find_top_action_index(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_action(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_repeat(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);
void play_action_ref(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

int get_times_value(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]);

bool find_child_element_of_type(Interpreter* interpreter, int parent_index, BULLETML_ELEMENT_TYPE type, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);
bool find_element_by_label(Interpreter* interpreter, BULLETML_ELEMENT_TYPE type, char* label, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index);

#endif // INTERPRETER_H
