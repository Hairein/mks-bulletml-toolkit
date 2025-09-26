#ifndef BULLET_H
#define BULLET_H

#include "mksbmli-defines.h"
#include "bulletml-base.h"

typedef struct {
    BULLETML_ELEMENT_TYPE type;
    BulletmlBase* parent;

    char label[MKSBMLI_MAX_TEXT_LENGTH];
} Bullet;

void init_bullet(Bullet* bullet, BulletmlBase* parent, const char* label);

void dump_bullet(Bullet* bullet);

#endif // BULLET_H
