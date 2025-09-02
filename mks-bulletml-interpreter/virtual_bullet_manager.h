#ifndef VIRTUAL_BULLET_MANAGER_H
#define VIRTUAL_BULLET_MANAGER_H

#include "mksbmli-defines.h"
#include "virtual_bullet.h"

typedef struct {
    MKSBMLI_BULLET_HANDLE next_free_bullet_handle;
    VirtualBullet bullets[MKSBMLI_MAX_BULLETS];
} VirtualBulletManager;

void init_virtual_bullet_manager(VirtualBulletManager* vbm);
void update_virtual_bullet_manager(VirtualBulletManager* vbm);

void get_vbm_bullets(VirtualBulletManager* vbm, int max_bullets, VirtualBullet** bullets, int* nos_bullets);
void destroy_vbm_bullets(VirtualBulletManager* vbm, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles);
void destroy_vbm_bullet(VirtualBulletManager* vbm, MKSBMLI_BULLET_HANDLE bullet_handle);
void clear_vbm_bullets(VirtualBulletManager* vbm);

MKSBMLI_BULLET_HANDLE spawn_virtual_bullet(VirtualBulletManager* vbm, int action_index, BULLETML_ATTRIBUTE_TYPE type, Vector2 position, float angle_degrees, float speed);

VirtualBullet* get_virtual_bullet_by_action_index(VirtualBulletManager* vbm, int action_index);

#endif // VIRTUAL_BULLET_MANAGER_H
