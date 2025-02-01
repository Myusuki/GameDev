#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <raylib.h>
#include <tmx.h>
#include "levelEntrance.h"

// structs
typedef struct Player
{
  Vector2 position;
  Rectangle hurtbox;
  Vector2 speed;
} Player;

void UpdatePlayer( tmx_map *map, Camera2D *camera, Player *player, CollisionObj *collisionList, Vector2 screenDim, float deltaTime );
void DrawPlayer( Player *player, tmx_map *map );

#endif
