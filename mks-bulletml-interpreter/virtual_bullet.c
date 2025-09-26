#include <stdbool.h>

#include "virtual_bullet.h"

void init_virtual_bullet(VirtualBullet* vb, MKSBMLI_BULLET_HANDLE handle, unsigned int action_id, BULLETML_ATTRIBUTE_TYPE bulletml_attribute, Vector2D position, float angle_degrees, float speed) {
    vb->handle = handle;

    vb->action_id = action_id;

    vb->bulletml_attribute = bulletml_attribute;

    vb->position = position;
    vb->angle_degrees = angle_degrees;
    vb->speed = speed;

    vb->velocity = (Vector2D){0.0f, 0.0f};

    vb->acceleration_vector = (Vector2D){0.0f, 0.0f};
    vb->acceleration_frames = 0;

    vb->changing_direction_interval = 0.0f;
    vb->changing_direction_frames = 0;

    vb->changing_speed_interval = 0.0f;
    vb->changing_speed_frames = 0;

    calculate_vb_velocity(vb);
}

void update_virtual_bullet(VirtualBullet* vb) {
    if(vb->handle == 0) return;

    if(vb->acceleration_frames > 0) {
        vb->position = Vector2DAdd(vb->position, vb->acceleration_vector);
        vb->acceleration_frames--;
    }

    bool recalculate_velocity = false;
    if(vb->changing_direction_frames > 0) {
        vb->angle_degrees += vb->changing_direction_interval;
        vb->changing_direction_frames--;

        recalculate_velocity = true;
    }
    if(vb->changing_speed_frames > 0) {
        vb->speed += vb->changing_speed_interval;
        vb->changing_speed_frames--;

        recalculate_velocity = true;
    }

    if(recalculate_velocity) calculate_vb_velocity(vb);

    vb->position = Vector2DAdd(vb->position, vb->velocity);
}

void calculate_vb_velocity(VirtualBullet* vb) {
    if(vb->handle == 0) return;

    float radians = -RADIANS_FACTOR * vb->angle_degrees;
    Vector2D unit_direction_vector = (Vector2D){1.0f, 0.0f};
    if(vb->bulletml_attribute == BULLETML_ATTRIBUTE_TYPE_VERTICAL) {
        unit_direction_vector = (Vector2D){0.0f, -1.0f};
    }
    Vector2D direction_vector = Vector2DNormalize(Vector2DRotate(unit_direction_vector, radians));

    vb->velocity = Vector2DScale(direction_vector, vb->speed);
}

void set_virtual_bullet_acceleration(VirtualBullet* vb, Vector2D acceleration_vector, unsigned int frames) {
    vb->acceleration_vector = acceleration_vector;
    vb->acceleration_frames = frames;
}

void set_virtual_bullet_changing_direction(VirtualBullet* vb, float new_direction_degrees, unsigned int frames) {
    vb->changing_direction_interval = (new_direction_degrees - vb->angle_degrees) / frames;
    vb->changing_direction_frames = frames;
}

void set_virtual_bullet_changing_speed(VirtualBullet* vb, float new_speed, unsigned int frames) {
    vb->changing_speed_interval = (new_speed - vb->speed) / frames;
    vb->changing_speed_frames = frames;
}
