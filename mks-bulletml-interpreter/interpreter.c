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
}

void reset_playhead(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    interpreter->can_play_frame = true;
    interpreter->top_action_index = 0;
    interpreter->play_frame_finished = false;

    interpreter->bulletml_attribute = BULLETML_ATTRIBUTE_TYPE_HORIZONTAL;
    BULLETML_ELEMENT_TYPE element_type = bulletml_bases[0]->type;
    if(element_type != BULLETML_ELEMENT_TYPE_BULLETML) {
        interpreter->can_play_frame = false;
        return;
    }

    Bulletml* bulletml = (Bulletml*)bulletml_bases[0];
    interpreter->bulletml_attribute = bulletml->attribute;

    interpreter->can_play_frame = find_top_action_index(interpreter, bulletml_bases);
}

void play_frame(Interpreter* interpreter, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    if(!interpreter->can_play_frame) return;

    if(interpreter->play_frame_finished) reset_playhead(interpreter, bulletml_bases);

    play_action(interpreter, 0, interpreter->top_action_index, 0, bulletml_bases);
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

void play_action(Interpreter* interpreter, int parent_action_index, int index, unsigned int offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    printf("current: Action\n");

    int start_child_index = interpreter->top_action_index + 1;

    if(interpreter->is_waiting && interpreter->waiting_action_index == index) {
        printf("waiting: %d\n", interpreter->wait_frames);

        interpreter->wait_frames--;
        if(interpreter->wait_frames > 0) return;

        interpreter->is_waiting = false;

        start_child_index = interpreter->waiting_index + 1;
    }

    for(int child_index = start_child_index; child_index < MKSBMLI_MAX_ELEMENTS; child_index++) {
        if(bulletml_bases[child_index] == NULL) continue;

        BULLETML_ELEMENT_TYPE element_type = bulletml_bases[child_index]->type;

        if(parent_action_index != 0 && bulletml_bases[child_index]->parent != bulletml_bases[index]) continue;

        switch(element_type) {
        case BULLETML_ELEMENT_TYPE_REPEAT: {
            printf("child: Repeat\n");

            play_repeat(interpreter, index, child_index, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE: {
            printf("child: Fire\n");

            play_fire(interpreter, index, child_index, offset, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_FIRE_REF: {
            printf("child: FireRef\n");

            play_fire_ref(interpreter, index, child_index, offset, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_CHANGE_SPEED: {
            printf("child: Speed\n");

            play_change_speed(interpreter, index, child_index, offset, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_CHANGE_DIRECTION: {
            printf("child: Direction\n");

            play_change_direction(interpreter, index, child_index, offset, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACCEL: {
            printf("child: Accel\n");

            play_accel(interpreter, index, child_index, offset, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_WAIT: {
            printf("child: Wait\n");

            play_wait(interpreter, index, child_index, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_VANISH: {
            printf("child: Vanish\n");

            play_vanish(interpreter, index, child_index, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION: {
            printf("child: Action\n");

            play_action(interpreter, index, child_index, offset, bulletml_bases);
        };
            break;
        case BULLETML_ELEMENT_TYPE_ACTION_REF: {
            printf("child: ActionRef\n");

            play_action_ref(interpreter, index, child_index, offset, bulletml_bases);
        };
            break;
        default: break;
        }
    }

    interpreter->play_frame_finished = true;
}

void play_repeat(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    Repeat* repeat = (Repeat*)bulletml_bases[index];
    int times = get_times_value(interpreter, index, bulletml_bases);

    int action_index, action_ref_index;
    bool has_action = find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_ACTION, bulletml_bases, &action_index);
    bool has_action_ref = find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_ACTION, bulletml_bases, &action_ref_index);

    for(int index = 0; index < times; index++) {
        if(has_action) play_action(interpreter, parent_action_index, action_index, (unsigned int) index, bulletml_bases);
        else if(has_action_ref) play_action_ref(interpreter, parent_action_index, action_ref_index, (unsigned int) index, bulletml_bases);
    }
}

void play_action_ref(Interpreter* interpreter, int parent_action_index, int index, unsigned int offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    ActionRef* action_ref= (ActionRef*)bulletml_bases[index];

    int action_index;
    if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_ACTION, action_ref->label, bulletml_bases, &action_index)) {
        play_action(interpreter, parent_action_index, action_index, offset, bulletml_bases);
    }
}

void play_fire(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    Vector2 position = (Vector2){0};
    float angle_degrees = 0.0f;
    float speed = 0.0f;

    VirtualBullet* parent_action_bullet = get_virtual_bullet_by_action_index(interpreter->vbm, parent_action_index);
    if(parent_action_bullet != NULL) {
        position = parent_action_bullet->position;
        angle_degrees = parent_action_bullet->angle_degrees;
        speed = parent_action_bullet->speed;
    }

    int bullet_index, bullet_ref_index;
    bool has_bullet = find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_BULLET, bulletml_bases, &bullet_index);
    bool has_bullet_ref = find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_BULLET_REF, bulletml_bases, &bullet_ref_index);

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

    Fire* fire = (Fire*)bulletml_bases[index];

    AARS_TYPE fire_direction_type;
    float fire_direction = get_direction(interpreter, index, bulletml_bases, &fire_direction_type);
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
    float fire_speed = get_speed(interpreter, index, bulletml_bases, &fire_speed_type);
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

    spawn_virtual_bullet(interpreter->vbm, parent_action_index, interpreter->bulletml_attribute, position, angle_degrees, speed);
}

void play_fire_ref(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    FireRef* fire_ref= (FireRef*)bulletml_bases[index];

    int fire_index;
    if(find_element_by_label(interpreter, BULLETML_ELEMENT_TYPE_FIRE, fire_ref->label, bulletml_bases, &fire_index)) {
        play_fire(interpreter, parent_action_index, fire_index, action_offset, bulletml_bases);
    }
}

void play_wait(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    Wait* wait = (Wait*)bulletml_bases[index];

    unsigned int frames = evaluate_bml_number_as_unsigned_int(&wait->content);

    interpreter->is_waiting = true;
    interpreter->waiting_index = index;
    interpreter->waiting_action_index = parent_action_index;
    interpreter->wait_frames = frames;

    printf("start wait: %d\n", interpreter->wait_frames);
}

void play_change_speed(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    ChangeSpeed* change_speed = (ChangeSpeed*)bulletml_bases[index];

    int new_speed_index;
    if(!find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_SPEED, bulletml_bases, &new_speed_index)) return;
    ARS_TYPE speed_type;
    float new_speed = get_speed(interpreter, index, bulletml_bases, &speed_type);

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, index, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_action_index(interpreter->vbm, index);
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

void play_change_direction(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    ChangeDirection* change_speed = (ChangeDirection*)bulletml_bases[index];

    int new_direction_index;
    if(!find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_DIRECTION, bulletml_bases, &new_direction_index)) return;
    AARS_TYPE direction_type;
    float new_direction = get_direction(interpreter, index, bulletml_bases, &direction_type);

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, index, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_action_index(interpreter->vbm, index);
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

void play_accel(Interpreter* interpreter, int parent_action_index, int index, int action_offset, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    Accel* change_speed = (Accel*)bulletml_bases[index];

    int new_horizontal_index;
    if(!find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_HORIZONTAL, bulletml_bases, &new_horizontal_index)) return;
    ARS_TYPE horizontal_type;
    float new_horizontal_value = get_horizontal(interpreter, index, bulletml_bases, &horizontal_type);

    int new_vertical_index;
    if(!find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_VERTICAL, bulletml_bases, &new_vertical_index)) return;
    ARS_TYPE vertical_type;
    float new_vertical_value = get_vertical(interpreter, index, bulletml_bases, &vertical_type);
    switch(vertical_type) {
    default:break;
    }

    int frames_index;
    unsigned int frames = 1;
    bool has_frames = find_child_element_of_type(interpreter, index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &frames_index);
    if(has_frames) {
        frames = get_term_value(interpreter, index, bulletml_bases);
    }

    VirtualBullet* vb = get_virtual_bullet_by_action_index(interpreter->vbm, index);
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

void play_vanish(Interpreter* interpreter, int parent_action_index, int index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]){
    //Vanish* vanish = (Vanish*)bulletml_bases[index];

    VirtualBullet* vb = get_virtual_bullet_by_action_index(interpreter->vbm, index);
    if(vb != NULL) {
        destroy_vbm_bullet(interpreter->vbm, vb->handle);
    }
}

int get_times_value(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int result = 1;

    int index;
    if(find_child_element_of_type(interpreter, parent_index, BULLETML_ELEMENT_TYPE_TIMES, bulletml_bases, &index)) {
        Times* times = (Times*)bulletml_bases[index];
        result = evaluate_bml_number_as_unsigned_int(&times->contents);
    }

    return result;
}

int get_term_value(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS]) {
    int result = 1;

    int index;
    if(find_child_element_of_type(interpreter, parent_index, BULLETML_ELEMENT_TYPE_TERM, bulletml_bases, &index)) {
        Term* term = (Term*)bulletml_bases[index];
        result = evaluate_bml_number_as_unsigned_int(&term->contents);
    }

    return result;
}

float get_direction(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], AARS_TYPE* direction_type) {
    *direction_type = AARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_index, BULLETML_ELEMENT_TYPE_DIRECTION, bulletml_bases, &index)) {
        Direction* direction = (Direction*)bulletml_bases[index];
        *direction_type = direction->attribute;
        result = evaluate_bml_number_as_float(&direction->contents);
    }

    return result;
}

float get_speed(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* speed_type) {
    *speed_type = ARS_TYPE_ABSOLUTE;
    int result = 1.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_index, BULLETML_ELEMENT_TYPE_SPEED, bulletml_bases, &index)) {
        Speed* speed = (Speed*)bulletml_bases[index];
        *speed_type = speed->attribute;
        result = evaluate_bml_number_as_float(&speed->contents);
    }

    return result;
}

float get_horizontal(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* horizontal_type) {
    *horizontal_type = ARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_index, BULLETML_ELEMENT_TYPE_HORIZONTAL, bulletml_bases, &index)) {
        Horizontal* horizontal = (Horizontal*)bulletml_bases[index];
        *horizontal_type = horizontal->attribute;
        result = evaluate_bml_number_as_float(&horizontal->contents);
    }

    return result;
}

float get_vertical(Interpreter* interpreter, int parent_index, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], ARS_TYPE* vertical_type) {
    *vertical_type = ARS_TYPE_ABSOLUTE;
    int result = 0.0f;

    int index;
    if(find_child_element_of_type(interpreter, parent_index, BULLETML_ELEMENT_TYPE_VERTICAL, bulletml_bases, &index)) {
        Vertical* vertical = (Vertical*)bulletml_bases[index];
        *vertical_type = vertical->attribute;
        result = evaluate_bml_number_as_float(&vertical->contents);
    }

    return result;
}

bool find_child_element_of_type(Interpreter* interpreter, int parent_index, BULLETML_ELEMENT_TYPE type, BulletmlBase* bulletml_bases[MKSBMLI_MAX_ELEMENTS], int* found_index) {
    BulletmlBase* parent = bulletml_bases[parent_index];

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
