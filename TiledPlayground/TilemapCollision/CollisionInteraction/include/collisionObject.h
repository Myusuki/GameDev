#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include <raylib.h>

typedef struct CollisionObj
{
  Rectangle collisionBox;
  int collisionType;
} CollisionObj;

enum tileType{ BLANK_TILE = 0, WALL };

#endif
