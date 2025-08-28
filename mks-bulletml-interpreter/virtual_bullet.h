#ifndef VIRTUAL_BULLET_H
#define VIRTUAL_BULLET_H

#include "raylib.h"

#include "mks-bulletml-interpreter.h"

typedef struct {
    bool handle;

    Vector2 position;
    Vector2 angles_degrees;
    float speed;

    Vector2 directional_velocity;
    Vector2 angular_velocity;
} VirtualBullet;

void init_virtual_bullet(VirtualBullet* virtual_bullet, MKSBMLI_BULLET_HANDLE handle);
int update_virtual_bullet(VirtualBullet* virtual_bullet);

#endif // VIRTUAL_BULLET_H
