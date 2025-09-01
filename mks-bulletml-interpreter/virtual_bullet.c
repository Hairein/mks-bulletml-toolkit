#include "raylib.h"
#include "raymath.h"

#include "virtual_bullet.h"

void init_virtual_bullet(VirtualBullet* vb, MKSBMLI_BULLET_HANDLE handle, BULLETML_ATTRIBUTE_TYPE bulletml_attribute, Vector2 position, float angle_degrees, float speed) {
    vb->handle = handle;

    vb->bulletml_attribute = bulletml_attribute;

    vb->position = position;
    vb->angle_degrees = angle_degrees;
    vb->speed = speed;

    vb->velocity = (Vector2){0.0f, 0.0f};

    vb->acceleration_vector = (Vector2){0.0f, 0.0f};
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
        vb->position = Vector2Add(vb->position, vb->acceleration_vector);
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

    vb->position = Vector2Add(vb->position, vb->velocity);
}

void calculate_vb_velocity(VirtualBullet* vb) {
    if(vb->handle == 0) return;

    float radians = DEG2RAD * vb->angle_degrees;
    Vector2 unit_direction_vector = (Vector2){1.0f, 0.0f};
    if(vb->bulletml_attribute == BULLETML_ATTRIBUTE_TYPE_VERTICAL) {
        unit_direction_vector = (Vector2){0.0f, 1.0f};
    }
    Vector2 direction_vector = Vector2Normalize(Vector2Rotate(unit_direction_vector, radians));

    vb->velocity = Vector2Scale(direction_vector, vb->speed);
}
