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
#include "bml-number.h"
#include "virtual_bullet_manager.h"
#include "utility.h"
#include "interpreter.h"

void init_interpreter(Interpreter* interpreter, VirtualBulletManager* vbm) {
    interpreter->vbm = vbm;

    interpreter->player_position = (Vector2){0};

    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) clear_action_info_block(&interpreter->active_actions[index]);

    interpreter->next_free_action_id = 1;
}

void reset_playhead(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    interpreter->can_play_frame = true;
    interpreter->top_action_index = 0;

    interpreter->bulletml_attribute = BULLETML_ATTRIBUTE_TYPE_HORIZONTAL;
    BULLETML_ELEMENT_TYPE element_type = bulletml_bases[0]->type;
    if(element_type != BULLETML_ELEMENT_TYPE_BULLETML) {
        interpreter->can_play_frame = false;
        return;
    }

    Bulletml* bulletml = (Bulletml*)bulletml_bases[0];
    interpreter->bulletml_attribute = bulletml->attribute;

    interpreter->can_play_frame = find_top_action_index(interpreter, bulletml_bases);

    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) clear_action_info_block(&interpreter->active_actions[index]);

    interpreter->first_frame = true;
}

void play_frame(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    if(!interpreter->can_play_frame) return;

    if(check_actions_playback_finished(interpreter)) {
        reset_playhead(interpreter, bulletml_bases);
    }

    if(interpreter->first_frame == true) {
        interpreter->first_frame = false;

        insert_action_for_playback(interpreter, 0, interpreter->top_action_index);
    }

    for(int action_array_index = 0; action_array_index < MKSBMLI_MAX_ELEMENTS; action_array_index++) {
        ActionInfoBlock* block = &interpreter->active_actions[action_array_index];

        if(block->action_index == 0) continue;

        play_action(interpreter, block->parent_action_index, block->action_index, block->action_index, 0, block, bulletml_bases);
    }
}

void insert_action_for_playback(Interpreter* interpreter, int parent_action_index, int action_index) {
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        if(interpreter->active_actions[index].action_index == 0) {
            init_action_info_block(&interpreter->active_actions[index], parent_action_index, action_index, 0);
            break;
        }
    }
}

void remove_action_from_playback(Interpreter* interpreter, int action_index) {
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        if(interpreter->active_actions[index].action_index == action_index) {
            interpreter->active_actions[index].action_index = 0;
            break;
        }
    }
}

bool check_actions_playback_finished(Interpreter* interpreter) {
    for(int index = 0; index < MKSBMLI_MAX_ELEMENTS; index++) {
        if(interpreter->active_actions[index].action_index != 0) return false;
    }

    return true;
}

bool find_top_action_index(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    for(int next_index = 1; next_index < MKSBMLI_MAX_ELEMENTS; next_index++) {
        BULLETML_ELEMENT_TYPE element_type = bulletml_bases[next_index]->type;
        if(element_type == BULLETML_ELEMENT_TYPE_ACTION) {
            interpreter->top_action_index = next_index;
            return true;
        }
    }

    return false;
}

void play_action(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, unsigned int offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    BulletmlBase* current_element = bulletml_bases[element_index];
    Action* current_action = (Action*)current_element;

    printf("play current: Action(%s)\n", current_action->label);

    int start_child_index = element_index + 1;

    if(action_info_block->is_waiting) {
        printf("waiting: %d\n", action_info_block->wait_frames);

        action_info_block->wait_frames--;
        if(action_info_block->wait_frames > 0) return;

        clear_waiting(action_info_block);

        start_child_index = action_info_block->wait_index + 1;
    }

    for(int child_index = start_child_index; child_index < MKSBMLI_MAX_ELEMENTS; child_index++) {
        BulletmlBase* child_element = bulletml_bases[child_index];

        if(child_element == NULL) continue;
        if(child_element->parent != current_element) continue;

        BULLETML_ELEMENT_TYPE child_element_type = child_element->type;
        switch(child_element_type) {
        case BULLETML_ELEMENT_TYPE_REPEAT: {
            printf("play child: Repeat\n");

            play_repeat(interpreter, parent_action_index, action_index, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE: {
            printf("play child: Fire\n");

            play_fire(interpreter, parent_action_index, action_index, child_index, offset, action_info_block,bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE_REF: {
            printf("play child: FireRef\n");

            play_fire_ref(interpreter, parent_action_index, action_index, child_index, offset, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_CHANGE_SPEED: {
            printf("play child: Speed\n");

            play_change_speed(interpreter, parent_action_index, action_index, child_index, offset, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_CHANGE_DIRECTION: {
            printf("play child: Direction\n");

            play_change_direction(interpreter, parent_action_index, action_index, child_index, offset, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACCEL: {
            printf("play child: Accel\n");

            play_accel(interpreter, parent_action_index, action_index, child_index, offset, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_WAIT: {
            printf("play child: Wait\n");

            play_wait(interpreter, parent_action_index, action_index, child_index, action_info_block, bulletml_bases);
            return;
        };
            break;
        case BULLETML_ELEMENT_TYPE_VANISH: {
            printf("play child: Vanish\n");

            play_vanish(interpreter, parent_action_index, action_index, child_index, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION: {
            printf("play child: Action\n");

            play_action(interpreter, element_index, action_index, child_index, offset, action_info_block, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION_REF: {
            printf("play child: ActionRef\n");

            play_action_ref(interpreter, parent_action_index, action_index, child_index, offset, action_info_block, bulletml_bases);
        };
            break;
        default: break;
        }
    }

    if(!action_info_block->is_waiting) remove_action_from_playback(interpreter, action_index);
}

void play_repeat(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    Repeat* repeat = (Repeat*)bulletml_bases[element_index];
    int times = get_times_value(interpreter, element_index, bulletml_bases);

    int found_action_index, found_action_ref_index;
    bool has_action = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_ACTION, bulletml_bases, &found_action_index);
    bool has_action_ref = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_ACTION_REF, bulletml_bases, &found_action_ref_index);

    for(int loop_index = 0; loop_index < times; loop_index++) {
        if(has_action) play_action(interpreter, parent_action_index, action_index, found_action_index, (unsigned int) loop_index, action_info_block, bulletml_bases);
        else if(has_action_ref) play_action_ref(interpreter, parent_action_index, action_index, found_action_ref_index, (unsigned int) loop_index, action_info_block, bulletml_bases);
    }
}

void play_action_ref(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, unsigned int offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    ActionRef* action_ref= (ActionRef*)bulletml_bases[element_index];

    int found_action_index;
    if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_ACTION, action_ref->label, bulletml_bases, &found_action_index)) {
        play_action(interpreter, parent_action_index, found_action_index, found_action_index, offset, action_info_block, bulletml_bases);
    }
}

void play_fire(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    Vector2 position = (Vector2){0};
    float angle_degrees = 0.0f;
    float speed = 0.0f;

    if(parent_action_index != 0) {
        for(int parent_info_index = 0; parent_info_index < MKSBMLI_MAX_ELEMENTS; parent_info_index++) {
            ActionInfoBlock* block = &interpreter->active_actions[parent_info_index];
            if(block->action_index != 0 && block->action_index == parent_action_index) {
                VirtualBullet* parent_action_bullet = get_virtual_bullet_by_action_id(interpreter->vbm, block->action_id);
                if(parent_action_bullet != NULL) {
                    position = parent_action_bullet->position;
                    angle_degrees = parent_action_bullet->angle_degrees;
                    speed = parent_action_bullet->speed;
                }
                break;
            }
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
        BulletRef* bullet_ref =  (BulletRef*)bulletml_bases[bullet_ref_index];
        if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_BULLET, bullet_ref->label, bulletml_bases, &found_bullet_index)) {
            bullet = (Bullet*)bulletml_bases[found_bullet_index];
        }
    }
    if(bullet == NULL) return;

    AARS_TYPE bullet_direction_type;
    float bullet_direction = get_direction(interpreter, found_bullet_index, bulletml_bases, &bullet_direction_type);
    switch(bullet_direction_type) {
    case AARS_TYPE_AIM: {
        angle_degrees = calc_angle_degrees(interpreter->bulletml_attribute, position, interpreter->player_position);
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
    float bullet_speed = get_speed(interpreter, found_bullet_index, bulletml_bases, &bullet_speed_type);
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
    float fire_direction = get_direction(interpreter, element_index, bulletml_bases, &fire_direction_type);
    switch(fire_direction_type) {
    case AARS_TYPE_AIM: {
        angle_degrees = calc_angle_degrees(interpreter->bulletml_attribute, position, interpreter->player_position);
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
    float fire_speed = get_speed(interpreter, element_index, bulletml_bases, &fire_speed_type);
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

    unsigned int new_action_id = interpreter->next_free_action_id++;
    spawn_virtual_bullet(interpreter->vbm, new_action_id, interpreter->bulletml_attribute, position, angle_degrees, speed);
    action_info_block->action_id = new_action_id;

    for(int bullet_child_action_index = found_bullet_index + 1; bullet_child_action_index < MKSBMLI_MAX_ELEMENTS; bullet_child_action_index++) {
        BulletmlBase* bullet_child =  bulletml_bases[bullet_child_action_index];
        if(bullet_child == NULL || bullet_child->parent != bulletml_bases[found_bullet_index]) continue;

        if(bullet_child->type == BULLETML_ELEMENT_TYPE_ACTION) {
            insert_action_for_playback(interpreter, action_index, bullet_child_action_index);

        } else if(bullet_child->type == BULLETML_ELEMENT_TYPE_ACTION_REF) {
            ActionRef* bullet_child_action_ref = (ActionRef*)bullet_child;
            int found_bullet_child_index = 0;
            if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_ACTION, bullet_child_action_ref->label, bulletml_bases, &found_bullet_child_index))
                insert_action_for_playback(interpreter, action_index, found_bullet_child_index);
        }
    }
}

void play_fire_ref(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    FireRef* fire_ref= (FireRef*)bulletml_bases[element_index];

    int fire_index;
    if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_FIRE, fire_ref->label, bulletml_bases, &fire_index)) {
        play_fire(interpreter, parent_action_index, action_index, fire_index, action_offset, action_info_block, bulletml_bases);
    }
}

void play_wait(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    Wait* wait = (Wait*)bulletml_bases[element_index];

    unsigned int frames = evaluate_bml_number_as_unsigned_int(&wait->content);

    set_waiting(action_info_block, frames, element_index);

    printf("start wait: %d\n", action_info_block->wait_frames);
}

void play_change_speed(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    ChangeSpeed* change_speed = (ChangeSpeed*)bulletml_bases[element_index];

    int new_speed_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_SPEED, bulletml_bases, &new_speed_index)) return;
    ARS_TYPE speed_type;
    float new_speed = get_speed(interpreter, element_index, bulletml_bases, &speed_type);

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, element_index, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_action_id(interpreter->vbm, action_info_block->action_id);
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

void play_change_direction(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    ChangeDirection* change_speed = (ChangeDirection*)bulletml_bases[element_index];

    int new_direction_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_DIRECTION, bulletml_bases, &new_direction_index)) return;
    AARS_TYPE direction_type;
    float new_direction = get_direction(interpreter, element_index, bulletml_bases, &direction_type);

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, element_index, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_action_id(interpreter->vbm, action_info_block->action_id);
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

void play_accel(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, int action_offset, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    Accel* change_speed = (Accel*)bulletml_bases[element_index];

    int new_horizontal_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_HORIZONTAL, bulletml_bases, &new_horizontal_index)) return;
    ARS_TYPE horizontal_type;
    float new_horizontal_value = get_horizontal(interpreter, element_index, bulletml_bases, &horizontal_type);

    int new_vertical_index;
    if(!find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_VERTICAL, bulletml_bases, &new_vertical_index)) return;
    ARS_TYPE vertical_type;
    float new_vertical_value = get_vertical(interpreter, element_index, bulletml_bases, &vertical_type);
    switch(vertical_type) {
    default:break;
    }

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, element_index, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_action_id(interpreter->vbm, action_info_block->action_id);
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

        Vector2 acceleration = (Vector2){new_horizontal_value, new_vertical_value};
        set_virtual_bullet_acceleration(vb, acceleration, frames);
    }
}

void play_vanish(Interpreter* interpreter, int parent_action_index, int action_index, int element_index, ActionInfoBlock* action_info_block, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    //Vanish* vanish = (Vanish*)bulletml_bases[index];

    VirtualBullet* vb = get_virtual_bullet_by_action_id(interpreter->vbm, action_info_block->action_id);
    if(vb != NULL) {
        destroy_vbm_bullet(interpreter->vbm, vb->handle);
    }
}

int get_times_value(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int result = 1;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_TIMES, bulletml_bases, &index)) {
        Times* times = (Times*)bulletml_bases[index];
        result = evaluate_bml_number_as_unsigned_int(&times->contents);
    }

    return result;
}

int get_term_value(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int result = 1;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &index)) {
        Term* term = (Term*)bulletml_bases[index];
        result = evaluate_bml_number_as_unsigned_int(&term->contents);
    }

    return result;
}

float get_direction(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], AARS_TYPE* direction_type) {
    *direction_type = AARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_DIRECTION, bulletml_bases, &index)) {
        Direction* direction = (Direction*)bulletml_bases[index];
        *direction_type = direction->attribute;
        result = evaluate_bml_number_as_float(&direction->contents);
    }

    return result;
}

float get_speed(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* speed_type) {
    *speed_type = ARS_TYPE_ABSOLUTE;
    int result = 1.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_SPEED, bulletml_bases, &index)) {
        Speed* speed = (Speed*)bulletml_bases[index];
        *speed_type = speed->attribute;
        result = evaluate_bml_number_as_float(&speed->contents);
    }

    return result;
}

float get_horizontal(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* horizontal_type) {
    *horizontal_type = ARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_HORIZONTAL, bulletml_bases, &index)) {
        Horizontal* horizontal = (Horizontal*)bulletml_bases[index];
        *horizontal_type = horizontal->attribute;
        result = evaluate_bml_number_as_float(&horizontal->contents);
    }

    return result;
}

float get_vertical(Interpreter* interpreter, int parent_element_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* vertical_type) {
    *vertical_type = ARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_element_index, BULLETML_ELEMENT_TYPE_VERTICAL, bulletml_bases, &index)) {
        Vertical* vertical = (Vertical*)bulletml_bases[index];
        *vertical_type = vertical->attribute;
        result = evaluate_bml_number_as_float(&vertical->contents);
    }

    return result;
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
