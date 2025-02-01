#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <raylib.h>
#include <tmx.h>

// structs
typedef struct Player
{
  Vector2 position;
  Rectangle hurtbox;
  Vector2 speed;
  bool collided;
} Player;

void UpdatePlayer( tmx_map *map, Camera2D *camera, Player *player, Vector2 screenDim, Rectangle mapRect, float deltaTime );
void DrawPlayer( Player *player, tmx_map *map );

#endif
