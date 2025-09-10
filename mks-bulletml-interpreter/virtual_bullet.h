#ifndef VIRTUAL_BULLET_H
#define VIRTUAL_BULLET_H

#include "mksbmli-defines.h"
#include "Vector2D.h"

typedef struct {
    MKSBMLI_BULLET_HANDLE handle;

    int action_id;

    BULLETML_ATTRIBUTE_TYPE bulletml_attribute;

    Vector2D position;
    float angle_degrees;
    float speed;

    Vector2D velocity;

    Vector2D acceleration_vector;
    unsigned int acceleration_frames;

    float changing_direction_interval;
    unsigned int changing_direction_frames;

    float changing_speed_interval;
    unsigned int changing_speed_frames;
} VirtualBullet;

void init_virtual_bullet(VirtualBullet* vb, MKSBMLI_BULLET_HANDLE handle, unsigned int action_id, BULLETML_ATTRIBUTE_TYPE bulletml_attribute, Vector2D position, float angle_degrees, float speed);
void update_virtual_bullet(VirtualBullet* vb);

void calculate_vb_velocity(VirtualBullet* vb);

void set_virtual_bullet_acceleration(VirtualBullet* vb, Vector2D acceleration_vector, unsigned int frames);
void set_virtual_bullet_changing_direction(VirtualBullet* vb, float new_direction_degrees, unsigned int frames);
void set_virtual_bullet_changing_speed(VirtualBullet* vb, float new_speed, unsigned int frames);

#endif // VIRTUAL_BULLET_H
