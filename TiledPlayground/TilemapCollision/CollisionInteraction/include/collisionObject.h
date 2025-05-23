#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include <raylib.h>

enum tileType{ BLANK_TILE = 0, WALL };
enum collisionShapeType { Rect = 0, Circ };
typedef struct // a circle collision shape for collision detection
{
  Vector2 centerPt;
  float radius;
} Circle;
typedef struct // general object for collision; to be used by other entities for collision purposes
{
  bool visible;
  int collisionType;
  int shape;
  union collisionShape
  {
    Rectangle rect;
    Circle circle;
  } collisionShape;
} CollisionObj;
typedef struct CollisionObjNode// CollisionObj node; used to make a linked list of CollisionObjs for use in quadtree
{
  CollisionObj *object;
  struct CollisionObjNode *next;
} CollisionObjNode;
#endif
