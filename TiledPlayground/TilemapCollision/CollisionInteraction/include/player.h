#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <raylib.h>
#include <tmx.h>
#include "collisionObject.h"

// structs
typedef struct
{
  Vector2 position;
  Rectangle hurtbox;
  Vector2 speed;
} Player;

void UpdatePlayer( Rectangle mapRect, Camera2D *camera, Player *player, Vector2 screenDim, float deltaTime );
void DrawPlayer( Player player );

#endif
