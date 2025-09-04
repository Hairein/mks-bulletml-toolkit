#include <stdio.h>

#include "virtual_bullet_manager.h"

void init_virtual_bullet_manager(VirtualBulletManager* vbm) {
    clear_vbm_bullets(vbm);
}

void update_virtual_bullet_manager(VirtualBulletManager* vbm) {
    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        if(vbm->bullets[index].handle == 0) continue;

        update_virtual_bullet(&vbm->bullets[index]);
    }
}

void get_vbm_bullets(VirtualBulletManager* vbm, int max_bullets, VirtualBullet** bullets, int* nos_bullets) {
    *nos_bullets = 0;
    int bullet_index = 0;

    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        if(vbm->bullets[index].handle == 0) continue;

        *nos_bullets = *nos_bullets + 1;

        bullets[bullet_index++] = &vbm->bullets[index];
    }
}

void destroy_vbm_bullets(VirtualBulletManager* vbm, MKSBMLI_BULLET_HANDLE* bullet_handles, int nos_bullet_handles) {
    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        if(vbm->bullets[index].handle == 0) continue;

        for(int delete_index = 0; delete_index < nos_bullet_handles; delete_index++) {
            if(vbm->bullets[index].handle == bullet_handles[delete_index]) {
                vbm->bullets[index].handle = 0;
                break;
            }
        }
    }
}

void destroy_vbm_bullet(VirtualBulletManager* vbm, MKSBMLI_BULLET_HANDLE bullet_handle) {
    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        if(vbm->bullets[index].handle == 0) continue;

        if(vbm->bullets[index].handle == bullet_handle) {
            vbm->bullets[index].handle = 0;
            break;
        }
    }
}

void clear_vbm_bullets(VirtualBulletManager* vbm) {
    vbm->next_free_bullet_handle = 1;
    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        vbm->bullets[index].handle = 0;
    }
}

MKSBMLI_BULLET_HANDLE spawn_virtual_bullet(VirtualBulletManager* vbm, int action_index, BULLETML_ATTRIBUTE_TYPE type, Vector2 position, float angle_degrees, float speed) {
    MKSBMLI_BULLET_HANDLE new_handle = (MKSBMLI_BULLET_HANDLE)0;

    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        if(vbm->bullets[index].handle != (MKSBMLI_BULLET_HANDLE)0) continue;

        new_handle = vbm->next_free_bullet_handle++;
        init_virtual_bullet(&vbm->bullets[index], new_handle, action_index, type, position, angle_degrees, speed);

        //printf("New vbullet spawned: %d\n", new_handle);
        break;
    }

    return new_handle;
}

VirtualBullet* get_virtual_bullet_by_action_id(VirtualBulletManager* vbm, unsigned int action_id) {
    VirtualBullet* result = NULL;

    if(action_id == 0) return result;

    for(int index = 0; index < MKSBMLI_MAX_BULLETS; index++) {
        if(vbm->bullets[index].handle == (MKSBMLI_BULLET_HANDLE)0) continue;

        if(vbm->bullets[index].action_id != action_id) continue;

        result = &vbm->bullets[index];
        break;
    }

    return result;
}
