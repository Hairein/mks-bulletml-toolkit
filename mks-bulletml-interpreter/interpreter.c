#include <stdio.h>
#include <string.h>

#include "bulletml.h"
#include "times.h"
#include "repeat.h"
#include "action-ref.h"
#include "fire-ref.h"
#include "bullet.h"
#include "action.h"
#include "fire.h"
#include "bullet-ref.h"
#include "action-ref.h"
#include "fire-ref.h"
#include "speed.h"
#include "wait.h"
#include "direction.h"
#include "change-speed.h"
#include "change-direction.h"
#include "accel.h"
//#include "vanish.h"
#include "horizontal.h"
#include "vertical.h"
#include "term.h"
#include "param.h"
#include "bml-number.h"
#include "virtual_bullet_manager.h"
#include "utility.h"
#include "interpreter.h"

void init_interpreter(Interpreter* interpreter, VirtualBulletManager* vbm) {
    interpreter->vbm = vbm;

    interpreter->emitter_position = (Vector2D){0};
    interpreter->player_position = (Vector2D){0};
    interpreter->rank = 0.5f;

    interpreter->next_free_bullet_id = 1;
}

void reset_playhead(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
   for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) clear_action_info_block(&interpreter->active_actions[index]);

   Bulletml* bulletml = (Bulletml*)bulletml_bases[0];
   interpreter->bulletml_attribute = bulletml->attribute;
}

void play_frame(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    interpreter->first_frame = check_actions_playback_finished(interpreter);
    if(interpreter->first_frame) {
        printf("Playing first frame\n");
        interpreter->first_frame = false;

        reset_playhead(interpreter, bulletml_bases);

        int top_action_index = find_top_action_index(interpreter, bulletml_bases);
        if(top_action_index != -1) insert_action_for_playback(interpreter, -1, top_action_index, 0, -1, NULL, 0);
    }

    for(int current_block_index = 0; current_block_index < MKSBMLI_MAX_ELEMENTS; current_block_index++) {
        ActionInfoBlock* current_block = &interpreter->active_actions[current_block_index];
        if(current_block->action_index == -1) break;
        if(current_block->action_index != -1 && current_block->is_finished) continue;

        play_action(interpreter, current_block, bulletml_bases);
    }
}

void play_action(Interpreter* interpreter, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int action_index = action_info_block->action_index;
    BulletmlBase* action_base = bulletml_bases[action_index];
    Action* action = (Action*)bulletml_bases[action_index];

    int parent_action_index = action_info_block->parent_action_index;
    if(parent_action_index != -1) {
        BulletmlBase* parent_action_base = bulletml_bases[parent_action_index];
        Action* parent_action = (Action*)bulletml_bases[parent_action_index];

        printf("Play action[%s] at index(%d), parent action[%s] at index(%d)\n",
               action->label, action_index,
               parent_action->label, parent_action_index);
    } else {
        printf("Play action[%s] at index(%d), no parent \n",
               action->label, action_index);
    }

    int next_child_index = action_index + 1;
    if(action_info_block->is_waiting) {
        if(action_info_block->wait_frames > 0) {
            printf("waiting: %d\n", action_info_block->wait_frames);
            action_info_block->wait_frames--;
            return;
        }

        action_info_block->is_waiting = false;
        next_child_index = action_info_block->wait_element_index + 1;
    }

    for(int child_index = next_child_index; child_index < MKSBMLI_MAX_ELEMENTS; child_index++) {
        BulletmlBase* child_base = bulletml_bases[child_index];
        if(child_base == NULL) break;
        else if(child_base->parent != action_base) continue;

        BULLETML_ELEMENT_TYPE child_element_type = child_base->type;
        switch(child_element_type) {
        case BULLETML_ELEMENT_TYPE_REPEAT: {
            printf("play child: Repeat\n");
            play_repeat(interpreter, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE: {
            printf("play child: Fire\n");
            play_fire(interpreter, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE_REF: {
            printf("play child: FireRef\n");

            get_params_for_element(interpreter, child_index, action_info_block->action_params, action_info_block->nos_action_params, MKSBMLI_MAX_PARAMS, action_info_block->fire_params, &action_info_block->nos_fire_params, bulletml_bases);

            play_fire_ref(interpreter, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_CHANGE_SPEED: {
            printf("play child: Speed\n");
            play_change_speed(interpreter, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_CHANGE_DIRECTION: {
            printf("play child: Direction\n");
            play_change_direction(interpreter, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACCEL: {
            printf("play child: Accel\n");
            play_accel(interpreter, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_WAIT: {
            printf("play child: Wait\n");
            play_wait(interpreter, child_index, action_info_block, bulletml_bases);
            return;
        };
            break;
        case BULLETML_ELEMENT_TYPE_VANISH: {
            printf("play child: Vanish\n");
            play_vanish(interpreter, child_index, action_info_block, bulletml_bases);
            return;
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION: {
            printf("play child: Action\n");
            insert_action_for_playback(interpreter, action_index, child_index, 0, action_info_block->block_index, NULL, 0);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION_REF: {
            printf("play child: ActionRef\n");

            float sub_action_params[MKSBMLI_MAX_PARAMS];
            int nos_sub_action_params;
            get_params_for_element(interpreter, child_index, action_info_block->action_params, action_info_block->nos_action_params, MKSBMLI_MAX_PARAMS, sub_action_params, &nos_sub_action_params, bulletml_bases);

            ActionRef* action_ref= (ActionRef*)bulletml_bases[child_index];

            int action_element_index;
            if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_ACTION, action_ref->label, bulletml_bases, &action_element_index)) {
                insert_action_for_playback(interpreter, action_index, action_element_index, 0, action_info_block->block_index, sub_action_params, nos_sub_action_params);
            }
        };
            break;
        default: break;
        }
    }

    if(!action_info_block->is_waiting) action_info_block->is_finished = true;
}

void play_wait(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
        Wait* wait = (Wait*)bulletml_bases[element_index];
    unsigned int frames = evaluate_bml_number_as_unsigned_int(&wait->content, interpreter->rank, action_info_block->action_params, action_info_block->nos_action_params);

        action_info_block->is_waiting = true;
        action_info_block->wait_frames = frames;
        action_info_block->wait_element_index = element_index;

        printf("start wait: %d\n", action_info_block->wait_frames);
}

void play_fire_ref(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
        FireRef* fire_ref= (FireRef*)bulletml_bases[element_index];

        int fire_element_index;
        if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_FIRE, fire_ref->label, bulletml_bases, &fire_element_index)) {
            play_fire(interpreter, fire_element_index, action_info_block, bulletml_bases);
        }
}

void play_fire(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int action_index = action_info_block->action_index;
    unsigned int action_offset = action_info_block->offset;

    Vector2D position = (Vector2D){interpreter->emitter_position.x, interpreter->emitter_position.y};
    float angle_degrees = 0.0f;
    float speed = 0.0f;

    int parent_bullet_id = get_ancestor_bullet_id(action_info_block, interpreter->active_actions);
    if(parent_bullet_id != 0) {
        VirtualBullet* parent_action_bullet = get_virtual_bullet_by_bullet_id(interpreter->vbm, parent_bullet_id);
        if(parent_action_bullet != NULL) {
            position = parent_action_bullet->position;
            angle_degrees = parent_action_bullet->angle_degrees;
            speed = parent_action_bullet->speed;
        }
    }

    int bullet_index = 0, bullet_ref_index = 0;
    bool has_bullet = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_BULLET, bulletml_bases, &bullet_index);
    bool has_bullet_ref = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_BULLET_REF, bulletml_bases, &bullet_ref_index);

    Bullet* bullet = NULL;
    int found_bullet_index;
    if(has_bullet) {
        bullet = (Bullet*)bulletml_bases[bullet_index];
        found_bullet_index = bullet_index;
    } else if(has_bullet_ref) {
        get_params_for_element(interpreter, bullet_ref_index, action_info_block->action_params, action_info_block->nos_action_params, MKSBMLI_MAX_PARAMS, action_info_block->bullet_params, &action_info_block->nos_bullet_params, bulletml_bases);

        BulletRef* bullet_ref =  (BulletRef*)bulletml_bases[bullet_ref_index];
        if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_BULLET, bullet_ref->label, bulletml_bases, &found_bullet_index)) {
            bullet = (Bullet*)bulletml_bases[found_bullet_index];
        }
    }
    if(bullet == NULL) return;

    AARS_TYPE bullet_direction_type;
    float bullet_direction = get_direction(interpreter, found_bullet_index, action_info_block->bullet_params, action_info_block->nos_bullet_params, bulletml_bases, &bullet_direction_type);
    switch(bullet_direction_type) {
    case AARS_TYPE_AIM: {
        angle_degrees = bullet_direction + calc_angle_degrees(interpreter->bulletml_attribute, position, interpreter->player_position);
        //printf("bullet: player(%d, %d), angle_degrees: %d\n", (int)interpreter->player_position.x, (int)interpreter->player_position.y, (int)angle_degrees);
    }; break;
    case AARS_TYPE_ABSOLUTE: {
        angle_degrees = bullet_direction;
    }; break;
    case AARS_TYPE_RELATIVE: {
        angle_degrees += bullet_direction;
    }; break;
    case AARS_TYPE_SEQUENCE: {
        angle_degrees += bullet_direction * (1 + action_offset);
    }; break;
    default: break;
    }

    ARS_TYPE bullet_speed_type;
    float bullet_speed = get_speed(interpreter, found_bullet_index, action_info_block->bullet_params, action_info_block->nos_bullet_params, bulletml_bases, &bullet_speed_type);
    switch(bullet_speed_type) {
    case ARS_TYPE_ABSOLUTE: {
        speed = bullet_speed;
    }; break;
    case ARS_TYPE_RELATIVE: {
        speed += bullet_speed;
    }; break;
    case ARS_TYPE_SEQUENCE: {
        speed = bullet_speed * (1 + action_offset);
    }; break;
    default: break;
    }

    Fire* fire = (Fire*)bulletml_bases[element_index];

    AARS_TYPE fire_direction_type;
    float fire_direction = get_direction(interpreter, element_index, action_info_block->fire_params, action_info_block->nos_fire_params, bulletml_bases, &fire_direction_type);
    switch(fire_direction_type) {
    case AARS_TYPE_AIM: {
        angle_degrees = fire_direction + calc_angle_degrees(interpreter->bulletml_attribute, position, interpreter->player_position);
    }; break;
    case AARS_TYPE_ABSOLUTE: {
        angle_degrees = fire_direction;
    }; break;
    case AARS_TYPE_RELATIVE: {
        angle_degrees += fire_direction;
    }; break;
    case AARS_TYPE_SEQUENCE: {
        angle_degrees += fire_direction * (1 + action_offset);
    }; break;
    default: break;
    }

    ARS_TYPE fire_speed_type;
    float fire_speed = get_speed(interpreter, element_index, action_info_block->fire_params, action_info_block->nos_fire_params, bulletml_bases, &fire_speed_type);
    switch(fire_speed_type) {
    case ARS_TYPE_ABSOLUTE: {
        speed = fire_speed;
    }; break;
    case ARS_TYPE_RELATIVE: {
        speed += fire_speed;
    }; break;
    case ARS_TYPE_SEQUENCE: {
        speed = fire_speed * (1 + action_offset);
    }; break;
    default: break;
    }

    unsigned int new_bullet_id = interpreter->next_free_bullet_id++;
    spawn_virtual_bullet(interpreter->vbm, new_bullet_id, interpreter->bulletml_attribute, position, angle_degrees, speed);
    action_info_block->bullet_id = new_bullet_id;

    for(int bullet_child_action_index = found_bullet_index + 1; bullet_child_action_index < MKSBMLI_MAX_ELEMENTS; bullet_child_action_index++) {
        BulletmlBase* bullet_child =  bulletml_bases[bullet_child_action_index];
        if(bullet_child == NULL || bullet_child->parent != bulletml_bases[found_bullet_index]) break;

        if(bullet_child->type == BULLETML_ELEMENT_TYPE_ACTION) {
            insert_action_for_playback(interpreter, action_index, bullet_child_action_index, 0, action_info_block->block_index, NULL, 0);

        } else if(bullet_child->type == BULLETML_ELEMENT_TYPE_ACTION_REF) {
            ActionRef* bullet_child_action_ref = (ActionRef*)bullet_child;

            float sub_action_params[MKSBMLI_MAX_PARAMS];
            int nos_sub_action_params;
            get_params_for_element(interpreter, bullet_child_action_index, action_info_block->action_params, action_info_block->nos_action_params, MKSBMLI_MAX_PARAMS, sub_action_params, &nos_sub_action_params, bulletml_bases);

            int found_bullet_child_index = 0;
            if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_ACTION, bullet_child_action_ref->label, bulletml_bases, &found_bullet_child_index))
                insert_action_for_playback(interpreter, action_index, found_bullet_child_index, 0, bullet_child_action_index, sub_action_params, nos_sub_action_params);
        }
    }
}

void play_repeat(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    Repeat* repeat = (Repeat*)bulletml_bases[element_index];
    int times = get_times_value(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases);

    int found_action_index, found_action_ref_index;
    bool has_action = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_ACTION, bulletml_bases, &found_action_index);
    bool has_action_ref = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_ACTION_REF, bulletml_bases, &found_action_ref_index);

    for(int loop_index = 0; loop_index < times; loop_index++) {
        if(has_action) insert_action_for_playback(interpreter, action_info_block->action_index, found_action_index, loop_index, action_info_block->block_index, NULL, 0);
        else if(has_action_ref) {
            ActionRef* action_ref= (ActionRef*)bulletml_bases[found_action_ref_index];

            float sub_action_params[MKSBMLI_MAX_PARAMS];
            int nos_sub_action_params;
            get_params_for_element(interpreter, found_action_ref_index, action_info_block->action_params, action_info_block->nos_action_params, MKSBMLI_MAX_PARAMS, sub_action_params, &nos_sub_action_params, bulletml_bases);

            int action_element_index;
            if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_ACTION, action_ref->label, bulletml_bases, &action_element_index)) {
                insert_action_for_playback(interpreter, action_info_block->action_index, action_element_index, loop_index, action_info_block->block_index, sub_action_params, nos_sub_action_params);
            }
        }
    }
}

void play_change_speed(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    ChangeSpeed* change_speed = (ChangeSpeed*)bulletml_bases[element_index];

    int action_offset = action_info_block->offset;

    int new_speed_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_SPEED, bulletml_bases, &new_speed_index)) return;
    ARS_TYPE speed_type;
    float new_speed = get_speed(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases, &speed_type);

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_bullet_id(interpreter->vbm, action_info_block->bullet_id);
    if(vb != NULL) {
        switch(speed_type) {
        case ARS_TYPE_RELATIVE: {
            new_speed += vb->speed;
        }; break;
        case ARS_TYPE_SEQUENCE: {
            new_speed = vb->speed * (1 + action_offset);
        }; break;
        case ARS_TYPE_ABSOLUTE:
        default: break;
        }

        set_virtual_bullet_changing_speed(vb, new_speed, frames);
    }
}

void play_change_direction(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    ChangeDirection* change_speed = (ChangeDirection*)bulletml_bases[element_index];

    int action_offset = action_info_block->offset;

    int new_direction_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_DIRECTION, bulletml_bases, &new_direction_index)) return;
    AARS_TYPE direction_type;
    float new_direction = get_direction(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases, &direction_type);

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_bullet_id(interpreter->vbm, action_info_block->bullet_id);
    if(vb != NULL) {
        switch(direction_type) {
        case AARS_TYPE_AIM: {
            new_direction += calc_angle_degrees(interpreter->bulletml_attribute, vb->position, interpreter->player_position);
        }; break;
        case AARS_TYPE_RELATIVE: {
            new_direction += vb->angle_degrees;
        }; break;
        case AARS_TYPE_SEQUENCE: {
            new_direction = vb->angle_degrees * (1 + action_offset);
        }; break;
        case AARS_TYPE_ABSOLUTE:
        default: break;
        }

        set_virtual_bullet_changing_direction(vb, new_direction, frames);
    }
}

void play_accel(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    Accel* change_speed = (Accel*)bulletml_bases[element_index];

    int action_offset = action_info_block->offset;

    int new_horizontal_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_HORIZONTAL, bulletml_bases, &new_horizontal_index)) return;
    ARS_TYPE horizontal_type;
    float new_horizontal_value = get_horizontal(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases, &horizontal_type);

    int new_vertical_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_VERTICAL, bulletml_bases, &new_vertical_index)) return;
    ARS_TYPE vertical_type;
    float new_vertical_value = get_vertical(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases, &vertical_type);
    switch(vertical_type) {
    default:break;
    }

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, element_index, action_info_block->action_params, action_info_block->nos_action_params, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_bullet_id(interpreter->vbm, action_info_block->bullet_id);
    if(vb != NULL) {
        switch(horizontal_type) {
        case ARS_TYPE_RELATIVE: {
            new_horizontal_value += vb->acceleration_vector.x;
        }; break;
        case ARS_TYPE_SEQUENCE: {
            new_horizontal_value = vb->acceleration_vector.x * (1 + action_offset);
        }; break;
        case ARS_TYPE_ABSOLUTE:
        default:break;
        }

        switch(vertical_type) {
        case ARS_TYPE_RELATIVE: {
            new_vertical_value += vb->acceleration_vector.x;
        }; break;
        case ARS_TYPE_SEQUENCE: {
            new_vertical_value = vb->acceleration_vector.x * (1 + action_offset);
        }; break;
        case ARS_TYPE_ABSOLUTE:
        default:break;
        }

        Vector2D acceleration = (Vector2D){new_horizontal_value, new_vertical_value};
        set_virtual_bullet_acceleration(vb, acceleration, frames);
    }
}

void play_vanish(Interpreter* interpreter, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    //Vanish* vanish = (Vanish*)bulletml_bases[index];

    VirtualBullet* vb = get_virtual_bullet_by_bullet_id(interpreter->vbm, action_info_block->bullet_id);
    if(vb != NULL) {
        destroy_vbm_bullet(interpreter->vbm, vb->handle);
    }
}

int find_top_action_index(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    for(int next_index = 1; next_index < MKSBMLI_MAX_ELEMENTS; next_index++) {
        BULLETML_ELEMENT_TYPE element_type = bulletml_bases[next_index]->type;
        if(element_type == BULLETML_ELEMENT_TYPE_ACTION) {
            return next_index;
        }
    }

    return -1;
}

void insert_action_for_playback(Interpreter* interpreter, int parent_action_index, int action_index, unsigned int offset, int parent_block_index, float* params, int nos_params) {
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        if(interpreter->active_actions[index].action_index == -1) {
            init_action_info_block(&interpreter->active_actions[index], parent_action_index, action_index, offset, parent_block_index, index);
            set_action_info_block_action_params(&interpreter->active_actions[index], params, nos_params);
            break;
        }
    }
}

bool check_actions_playback_finished(Interpreter* interpreter) {
    for(int block_index = 0; block_index < MKSBMLI_MAX_ELEMENTS; block_index++) {
        ActionInfoBlock* current_block = &interpreter->active_actions[block_index];
        if(current_block->action_index != -1 && !current_block->is_finished) return false;
    }

    return true;
}

float get_direction(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], AARS_TYPE* direction_type) {
    *direction_type = AARS_TYPE_ABSOLUTE;
    float result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_DIRECTION, bulletml_bases, &index)) {
        Direction* direction = (Direction*)bulletml_bases[index];
        *direction_type = direction->attribute;
        result = evaluate_bml_number_as_float(&direction->contents, interpreter->rank, params, nos_params);
    }

    return result;
}

float get_speed(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* speed_type) {
    *speed_type = ARS_TYPE_ABSOLUTE;
    float result = 1.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_SPEED, bulletml_bases, &index)) {
        Speed* speed = (Speed*)bulletml_bases[index];
        *speed_type = speed->attribute;
        result = evaluate_bml_number_as_float(&speed->contents, interpreter->rank, params, nos_params);
    }

    return result;
}

int get_times_value(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int result = 1;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_TIMES, bulletml_bases, &index)) {
        Times* times = (Times*)bulletml_bases[index];
        result = evaluate_bml_number_as_unsigned_int(&times->contents, interpreter->rank, params, nos_params);
    }

    return result;
}

int get_term_value(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int result = 1;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &index)) {
        Term* term = (Term*)bulletml_bases[index];
        result = evaluate_bml_number_as_unsigned_int(&term->contents, interpreter->rank, params, nos_params);
    }

    return result;
}

float get_horizontal(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* horizontal_type) {
    *horizontal_type = ARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_HORIZONTAL, bulletml_bases, &index)) {
        Horizontal* horizontal = (Horizontal*)bulletml_bases[index];
        *horizontal_type = horizontal->attribute;
        result = evaluate_bml_number_as_float(&horizontal->contents, interpreter->rank, params, nos_params);
    }

    return result;
}

float get_vertical(Interpreter* interpreter, int parent_element_index, float* params, int nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* vertical_type) {
    *vertical_type = ARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_VERTICAL, bulletml_bases, &index)) {
        Vertical* vertical = (Vertical*)bulletml_bases[index];
        *vertical_type = vertical->attribute;
        result = evaluate_bml_number_as_float(&vertical->contents, interpreter->rank, params, nos_params);
    }

    return result;
}

bool find_element_by_label(Interpreter* interpreter, BULLETML_ELEMENT_TYPE type, char* label, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index) {
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        if(bulletml_bases[index] == NULL) return false;

        if(bulletml_bases[index]->type != type) continue;

        switch(type) {
        case BULLETML_ELEMENT_TYPE_BULLET: {
            Bullet* element = (Bullet*)bulletml_bases[index];
            if(strncmp(element->label,label, MKSBMLI_MAX_TEXT_LENGTH) == 0) {
                *found_index = index;
                return true;
            }
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION: {
            Action* element = (Action*)bulletml_bases[index];
            if(strncmp(element->label,label, MKSBMLI_MAX_TEXT_LENGTH) == 0) {
                *found_index = index;
                return true;
            }
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE: {
            Fire* element = (Fire*)bulletml_bases[index];
            if(strncmp(element->label,label, MKSBMLI_MAX_TEXT_LENGTH) == 0) {
                *found_index = index;
                return true;
            }
        };
            break;
        case BULLETML_ELEMENT_TYPE_BULLET_REF: {
            BulletRef* element = (BulletRef*)bulletml_bases[index];
            if(strncmp(element->label,label, MKSBMLI_MAX_TEXT_LENGTH) == 0) {
                *found_index = index;
                return true;
            }
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION_REF: {
            ActionRef* element = (ActionRef*)bulletml_bases[index];
            if(strncmp(element->label,label, MKSBMLI_MAX_TEXT_LENGTH) == 0) {
                *found_index = index;
                return true;
            }
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE_REF: {
            FireRef* fire_ref = (FireRef*)bulletml_bases[index];
            if(strncmp(fire_ref->label,label, MKSBMLI_MAX_TEXT_LENGTH) == 0) {
                *found_index = index;
                return true;
            }
        };
            break;
        default: break;
        }
    }

    return false;
}

bool find_child_element_of_type(Interpreter* interpreter, int parent_element_index, BULLETML_ELEMENT_TYPE type, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index) {
    BulletmlBase* parent = bulletml_bases[parent_element_index];

    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        BulletmlBase* child = bulletml_bases[index];

        if(child == NULL) return false;
        if(child->type != type) continue;
        if(child->parent != parent) continue;

        *found_index = index;
        return true;
    }

    return false;
}

void get_params_for_element(Interpreter* interpreter, int parent_element_index,  float* action_params, int nos_action_params, int max_params, float* params, int* nos_params, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    *nos_params = 0;

    BulletmlBase* parent = bulletml_bases[parent_element_index];

    int count = 0;
    for(int index = parent_element_index + 1; index < MKSBMLI_MAX_ELEMENTS; index++) {
        BulletmlBase* child = bulletml_bases[index];
        if(child == NULL || child->type != BULLETML_ELEMENT_TYPE_PARAM || child->parent != parent) continue;

        Param* param = (Param*)child;

        float new_value = evaluate_bml_number_as_float(&param->contents, interpreter->rank, action_params, nos_action_params);
        params[count++] = new_value;

        if(count >= max_params) return;
    }

    *nos_params = count;
}

int get_ancestor_bullet_id(ActionInfoBlock* block, ActionInfoBlock* blocks) {
    if(block->parent_block_index == -1) return 0;

    ActionInfoBlock* parent_block = &blocks[block->parent_block_index];
    while(true) {
        if(parent_block->bullet_id != 0) return parent_block->bullet_id;
        if(parent_block->parent_block_index != -1) parent_block = &blocks[parent_block->parent_block_index];
        else break;
    }

    return 0;
}
