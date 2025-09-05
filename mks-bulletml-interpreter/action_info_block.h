#ifndef ACTION_INFO_BLOCK_H
#define ACTION_INFO_BLOCK_H

#include <stdbool.h>

typedef struct {
    int parent_action_index;
    int action_index;
    int action_element_index;

    unsigned int bullet_id;

    bool is_waiting;
    unsigned int wait_frames;
    int wait_element_index;

    bool is_finished;

    // int parent_action_index;
    // int action_index;

    // unsigned int action_id;

    // bool is_waiting;
    // unsigned int wait_frames;
    // int wait_index;
} ActionInfoBlock;

void init_action_info_block(ActionInfoBlock* block, int parent_action_index, int action_index);
// void init_action_info_block(ActionInfoBlock* block, int parent_action_index, int action_index, unsigned int action_id);
void clear_action_info_block(ActionInfoBlock* block);

// void set_waiting(ActionInfoBlock* block, int frames, int wait_index);
// void clear_waiting(ActionInfoBlock* block);

#endif // ACTION_INFO_BLOCK_H
