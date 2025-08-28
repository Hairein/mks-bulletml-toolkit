#include "utility.h"
#include "bullet.h"

void init_bullet(Bullet* bullet, BulletmlBase* parent, const char* label) {
    bullet->type = BULLETML_ELEMENT_TYPE_BULLET;
    bullet->parent = parent;

    copy_text(bullet->label, label);
}

void dump_bullet(Bullet* bullet) {
    printf("BULLET#label[%s]\n", bullet->label);
}
