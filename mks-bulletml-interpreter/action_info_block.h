#ifndef ACTION_INFO_BLOCK_H
#define ACTION_INFO_BLOCK_H

#include <stdbool.h>

#include "mksbmli-defines.h"

typedef struct {
    int parent_action_index;
    int action_index;
    int action_element_index;

    int parent_block_index;
    int block_index;

    unsigned int bullet_id;

    bool is_waiting;
    unsigned int wait_frames;
    int wait_element_index;

    bool is_finished;

    unsigned int offset;

    float action_params[MKSBMLI_MAX_PARAMS];
    int nos_action_params;

    float fire_params[MKSBMLI_MAX_PARAMS];
    int nos_fire_params;

    float bullet_params[MKSBMLI_MAX_PARAMS];
    int nos_bullet_params;
} ActionInfoBlock;

void init_action_info_block(ActionInfoBlock* block, int parent_action_index, int action_index, unsigned int offset, int parent_block_index, int block_index);
void clear_action_info_block(ActionInfoBlock* block);

void set_action_info_block_action_params(ActionInfoBlock* block, float* params, int nos_params);

#endif // ACTION_INFO_BLOCK_H
