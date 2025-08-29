#include "raylib.h"

#include "virtual_bullet.h"

void init_vb(VirtualBullet* vb, MKSBMLI_BULLET_HANDLE handle, Vector2 position, Vector2 angle_degrees, float speed) {
    vb->handle = handle;

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
}

void update_vb(VirtualBullet* vb) {
    if(vb->handle == 0) return;
}

void calculate_vb_velocity(VirtualBullet* vb) {
    if(vb->handle == 0) return;
}
