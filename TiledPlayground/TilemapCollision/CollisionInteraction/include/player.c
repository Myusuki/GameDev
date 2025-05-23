#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <tmx.h>
#include "player.h"

// func declarations
// player controls
void UpdatePlayerControls( Rectangle mapRect, Camera2D *camera, Player *player );
// camera
void UpdatePlayerCamera( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect );
// collision detection

// utility
bool CheckForFractionPortion( float number );
int DeleteFractionPortion( float number );

void UpdatePlayer( Rectangle mapRect, Camera2D *camera, Player *player, Vector2 screenDim, float deltaTime )
{
  UpdatePlayerControls(mapRect, camera, player);

  UpdatePlayerCamera(camera, player, (int)screenDim.x, (int)screenDim.y, mapRect);
}
void DrawPlayer( Player player )
{
  DrawRectangleRec(player.hurtbox, BLACK);
}

void UpdatePlayerCamera( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect )
{
  // camera boundary points
  Vector2 topLeftPoint = GetScreenToWorld2D( (Vector2){ 0, 0 }, *camera);
  // int cast all coordinates and +- 1 to perfectly bound camera
  if( CheckForFractionPortion(topLeftPoint.x) )
    topLeftPoint.x = DeleteFractionPortion(topLeftPoint.x) - 1;
  if( CheckForFractionPortion(topLeftPoint.y) )
    topLeftPoint.y = DeleteFractionPortion(topLeftPoint.y) - 1;
  Vector2 bottomRightPoint = GetScreenToWorld2D( (Vector2){ screenWidth, screenHeight }, *camera);
  if( CheckForFractionPortion(bottomRightPoint.x) )
    bottomRightPoint.x = DeleteFractionPortion(bottomRightPoint.x) + 1;
  if( CheckForFractionPortion(bottomRightPoint.y) )
    bottomRightPoint.y = DeleteFractionPortion(bottomRightPoint.y) + 1;
  Vector2 centerPoint = GetScreenToWorld2D( (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f }, *camera);

  // debug info
  printf( "Center Point X: %f, Y: %f\n", centerPoint.x, centerPoint.y );
  printf( "Map Dim. X: %f, Y: %f, W: %f, H: %f\n", mapRect.x, mapRect.y, mapRect.width, mapRect.height );
  printf( "TOP LEFT X: %f, Y: %f\n", topLeftPoint.x, topLeftPoint.y );
  printf( "BOTTOM RIGHT X: %f, Y: %f\n", bottomRightPoint.x, bottomRightPoint.y );
  printf( "PLAYER POS X: %f, Y: %f\n", player->position.x, player->position.y );

  // boundary checking 
  if( topLeftPoint.x > mapRect.x && player->position.x < centerPoint.x && player->speed.x < 0 )
    camera->target.x = player->position.x;
  if( bottomRightPoint.x < mapRect.width && player->position.x > centerPoint.x && player->speed.x > 0 )
    camera->target.x = player->position.x;
  if( topLeftPoint.y > mapRect.y && player->position.y < centerPoint.y && player->speed.y < 0 )
    camera->target.y = player->position.y;
  if( bottomRightPoint.y < mapRect.height && player->position.y > centerPoint.y && player->speed.y > 0 )
    camera->target.y = player->position.y;
}

void UpdatePlayerControls( Rectangle mapRect, Camera2D *camera, Player *player )
{
  // player controls
  // player directional controls
  if( IsKeyDown(KEY_LEFT) )
  {
    if( player->speed.x > 0 ) // change speed direction if necessary
      player->speed.x *= -1;
    if( player->hurtbox.x > mapRect.x )
    {
      player->position.x += (int)(player->speed.x * GetFrameTime() );
      player->hurtbox.x += (int)(player->speed.x * GetFrameTime() );
    }
  }
  else if( IsKeyDown(KEY_RIGHT) )
  {
    if( player->speed.x < 0 )
      player->speed.x *= -1;
    if( player->hurtbox.x + player->hurtbox.width < mapRect.width )
    {
      player->position.x += (int)(player->speed.x * GetFrameTime());
      player->hurtbox.x += (int)(player->speed.x * GetFrameTime());
    }
  }
  else if( IsKeyDown(KEY_UP) )
  {
    if( player->speed.y > 0 )
      player->speed.y *= -1;
    if( player->hurtbox.y > mapRect.y )
    {
      player->position.y += (int)(player->speed.y * GetFrameTime());
      player->hurtbox.y += (int)(player->speed.y * GetFrameTime());
    }
  }
  else if( IsKeyDown(KEY_DOWN) )
  {
    if( player->speed.y < 0 )
      player->speed.y *= -1;
    if( player->hurtbox.y + player->hurtbox.height < mapRect.height )
    {
      player->position.y += (int)(player->speed.y * GetFrameTime() );
      player->hurtbox.y += (int)(player->speed.y * GetFrameTime() );
    }
  }
  // reset player to map center
  if( IsKeyPressed(KEY_R) )
  {
    player->position = (Vector2){ .x = mapRect.width / 2, .y = mapRect.height / 2 };
    player->hurtbox.x = player->position.x - (player->hurtbox.width / 2);
    player->hurtbox.y = player->position.y - (player->hurtbox.height / 2);
    camera->target = player->position;
  }
}

bool CheckForFractionPortion( float number )
{
  float fractionChecker = number - (int)number;

  if( fractionChecker > 0 && fractionChecker < 1 )
    return true;
  else if( fractionChecker < 0 && fractionChecker > -1 )
    return true;

  return false;
}
int DeleteFractionPortion( float number )
{
  float fractionChecker = number - (int)number;
  int fractionPortionDeleted = (int)number;

  if( fractionChecker > 0 && fractionChecker < 1 )
    fractionPortionDeleted = (int)number;
  else if( fractionChecker < 0 && fractionChecker > -1 )
    fractionPortionDeleted = (int)number;

  return fractionPortionDeleted;
}
