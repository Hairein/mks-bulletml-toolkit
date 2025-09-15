#include "string.h"

#include "action_info_block.h"

void init_action_info_block(ActionInfoBlock* block, int parent_action_index, int action_index, unsigned int offset, int parent_block_index, int block_index) {
    block->parent_action_index = parent_action_index;
    block->action_index = action_index;

    block->parent_block_index = parent_block_index;
    block->block_index = block_index;

    block->action_element_index = 0;

    block->bullet_id = 0;

    block->is_waiting = false;
    block->wait_frames = 0;
    block->wait_element_index = -1;

    block->is_finished = false;

    block->offset = offset;

    memset(block->action_params, 0, sizeof(float) * MKSBMLI_MAX_PARAMS);
    block->nos_action_params = 0;

    memset(block->fire_params, 0, sizeof(float) * MKSBMLI_MAX_PARAMS);
    block->nos_fire_params = 0;

    memset(block->bullet_params, 0, sizeof(float) * MKSBMLI_MAX_PARAMS);
    block->nos_bullet_params = 0;
}

void clear_action_info_block(ActionInfoBlock* block) {
    block->parent_action_index = -1;
    block->action_index = -1;

    block->parent_block_index = -1;
    block->block_index = -1;

    block->bullet_id = 0;

    block->is_waiting = false;
    block->wait_frames = 0;
    block->wait_element_index = -1;

    block->is_finished = false;

    block->offset = 0;

    memset(block->action_params, 0, sizeof(float) * MKSBMLI_MAX_PARAMS);
    block->nos_action_params = 0;

    memset(block->fire_params, 0, sizeof(float) * MKSBMLI_MAX_PARAMS);
    block->nos_fire_params = 0;

    memset(block->bullet_params, 0, sizeof(float) * MKSBMLI_MAX_PARAMS);
    block->nos_bullet_params = 0;
}

void set_action_info_block_action_params(ActionInfoBlock* block, float* params, int nos_params) {
    memset(block->action_params, 0, sizeof(float) * MKSBMLI_MAX_PARAMS);
    if (params != NULL) {
        memcpy(block->action_params, params, sizeof(float) * nos_params);
    }
    block->nos_action_params = nos_params;
}
