#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

// structs
typedef struct Player
{
  Vector2 position;
  Rectangle hurtbox;
  Vector2 speed;
  bool canJump;
} Player;

// player related funcs
void UpdatePlayer( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect, float deltaTime );
void UpdatePlayerCamera( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect );
// utility funcs
bool CheckForFractionPortion( float number );
int DeleteFractionPortion( float number );
#endif
