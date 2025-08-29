#ifndef VIRTUAL_BULLET_H
#define VIRTUAL_BULLET_H

#include "raylib.h"

#include "mks-bulletml-interpreter.h"

typedef struct {
    bool handle;

    Vector2 position;
    Vector2 angle_degrees;
    float speed;

    Vector2 velocity;

    Vector2 acceleration_vector;
    unsigned int acceleration_frames;

    float changing_direction_interval;
    unsigned int changing_direction_frames;

    float changing_speed_interval;
    unsigned int changing_speed_frames;
} VirtualBullet;

void init_virtual_bullet(VirtualBullet* vb, MKSBMLI_BULLET_HANDLE handle, Vector2 position, Vector2 angle_degrees, float speed);
void update_virtual_bullet(VirtualBullet* vb);

void calculate_vb_velocity(VirtualBullet* vb);

#endif // VIRTUAL_BULLET_H
