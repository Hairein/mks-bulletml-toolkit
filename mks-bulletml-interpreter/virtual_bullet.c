#include "virtual_bullet.h"

void init_virtual_bullet(VirtualBullet* virtual_bullet, MKSBMLI_BULLET_HANDLE handle) {
    virtual_bullet->handle = handle;

    virtual_bullet->position = (Vector3){0.0f, 0.0f, 0.0f};
    virtual_bullet->angles_degrees = (Vector3){0.0f, 0.0f, 0.0f};
    virtual_bullet->speed = 0.0f;

    virtual_bullet->directional_velocity = (Vector3){0.0f, 0.0f, 0.0f};
    virtual_bullet->angular_velocity = (Vector3){0.0f, 0.0f, 0.0f};
}

int update_virtual_bullet(VirtualBullet* virtual_bullet) {
    return MKSBMLI_NO_ERROR;
}
