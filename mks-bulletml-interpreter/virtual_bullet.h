#ifndef VIRTUAL_BULLET_H
#define VIRTUAL_BULLET_H

#include "mks-bulletml-interpreter.h"

typedef struct {
    bool handle;

    Vector3 position;
    Vector3 angles_degrees;
    float speed;

    Vector3 directional_velocity;
    Vector3 angular_velocity;
} VirtualBullet;

void init_virtual_bullet(VirtualBullet* virtual_bullet, MKSBMLI_BULLET_HANDLE handle);
int update_virtual_bullet(VirtualBullet* virtual_bullet);

#endif // VIRTUAL_BULLET_H
