#include "action_info_block.h"

void init_action_info_block(ActionInfoBlock* block, int parent_action_index, int action_index, unsigned int action_id) {
    block->parent_action_index = parent_action_index;
    block->action_index = action_index;

    block->action_id = action_id;

    block->is_waiting = false;
    block->wait_frames = 0;
    block->wait_index = 0;
}

void clear_action_info_block(ActionInfoBlock* block) {
    block->parent_action_index = 0;
    block->action_index = 0;

    block->action_id = 0;

    block->is_waiting = false;
    block->wait_frames = 0;
    block->wait_index = 0;
}

void set_waiting(ActionInfoBlock* block, int frames, int wait_index) {
    block->is_waiting = true;
    block->wait_frames = frames;
    block->wait_index = wait_index;
}

void clear_waiting(ActionInfoBlock* block)
{
    block->is_waiting = false;
    block->wait_frames = 0;
}
