#include <raylib.h>
#include <stdint.h>
#include <stdio.h>
#include <tmx.h>
#include "player.h"

// func declarations
bool CheckForFractionPortion( float number );
int DeleteFractionPortion( float number );
int32_t getGID( tmx_layer *layer, unsigned int mapWidth, unsigned int column, unsigned int row );

void UpdatePlayerCamera( Camera2D *camera, Player *player, int screenWidth, int screenHeight, Rectangle mapRect );
bool DetectCollision( Player *player, tmx_map *map, int direction );
enum { LEFT = 1, RIGHT, UP, DOWN } direction;

void UpdatePlayer( tmx_map *map, Camera2D *camera, Player *player, Vector2 screenDim, Rectangle mapRect, float deltaTime )
{ 
  // player controls
  // player directional controls
  if( IsKeyDown(KEY_LEFT) )
  {
    if( player->speed.x > 0 ) // change speed direction
      player->speed.x *= -1;
    if( !DetectCollision(player, map, LEFT) && player->hurtbox.x > mapRect.x ) // collision & tilemap bounds check
    // if( player->hurtbox.x > mapRect.x ) // collision & tilemap bounds check
    {
      player->position.x += (int)(player->speed.x * deltaTime);
      player->hurtbox.x += (int)(player->speed.x * deltaTime);
    }
  }
  else if( IsKeyDown(KEY_RIGHT) )
  {
    if( player->speed.x < 0 )
      player->speed.x *= -1;
    if( !DetectCollision(player, map, RIGHT) && (player->hurtbox.x + player->hurtbox.width) < mapRect.width ) // bounds check
    // if( (player->hurtbox.x + player->hurtbox.width) < mapRect.width ) // bounds check
    {
      player->position.x += (int)(player->speed.x * deltaTime);
      player->hurtbox.x += (int)(player->speed.x * deltaTime);
    }
  }
  else if( IsKeyDown(KEY_UP) )
  {
    if( player->speed.y > 0 )
      player->speed.y *= -1;
    if( !DetectCollision(player, map, UP) && player->hurtbox.y > mapRect.y ) // bounds check
    // if( player->hurtbox.y > mapRect.y ) // bounds check
    {
      player->position.y += (int)(player->speed.y * deltaTime);
      player->hurtbox.y += (int)(player->speed.y * deltaTime);
    }
  }
  else if( IsKeyDown(KEY_DOWN) )
  {
    if( player->speed.y < 0 )
      player->speed.y *= -1;
    if( !DetectCollision(player, map, DOWN) && (player->hurtbox.y + player->hurtbox.height) < mapRect.height ) // bounds check
    // if( (player->hurtbox.y + player->hurtbox.height) < mapRect.height ) // bounds check
    {
      player->position.y += (int)(player->speed.y * deltaTime);
      player->hurtbox.y += (int)(player->speed.y * deltaTime);
    }
  }

  // reset player to map center
  if( IsKeyPressed(KEY_R) )
  {
    player->position = (Vector2){ mapRect.width / 2, mapRect.height / 2 };
    player->hurtbox.x = player->position.x - (player->hurtbox.width / 2);
    player->hurtbox.y = player->position.y - (player->hurtbox.height / 2);
    camera->target = player->position;
  }

  int xTile = (int)( ( (player->position.x) / map->tile_height) );
  int yTile = (int)( ( (player->position.y) / map->tile_height) );
  printf( "Player X Tile: %d\n", xTile );
  printf( "Player Y Tile: %d\n", yTile );

  UpdatePlayerCamera(camera, player, (int)screenDim.x, (int)screenDim.y, mapRect);
}
void DrawPlayer( Player *player, tmx_map *map )
{
  DrawRectangleRec(player->hurtbox, BLACK);
}

// func code
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
bool DetectCollision( Player *player, tmx_map *map, int direction )
{
  int xTile = 0, yTile = 0;
  int gid;
  tmx_tile *tile;
  bool collisionDetected = false;
  tmx_layer *layerHead = map->ly_head;
  while( layerHead )
  {
    printf( "Layer Name: %s\n", layerHead->name );
    switch( direction )
    {
      case LEFT:
          xTile = (int)( ( (player->position.x - (player->hurtbox.width / 2) - 1) / map->tile_width) );
          yTile = (int)( ( (player->position.y) / map->tile_height) );
          printf( "X Tile: %d\n", xTile );
          printf( "Y Tile: %d\n", yTile );
          
          if( xTile <= 0 )
            xTile = 0;
          else if( xTile >= map->width - 1 )
            xTile = map->width - 1;
          if( yTile <= 0 )
            yTile = 0;
          else if( yTile >= map->height - 1)
            yTile = map->height - 1;

          gid = getGID(layerHead, map->width, xTile, yTile);
          tile = map->tiles[gid];
          if( gid != 0 && tile->collision )
            collisionDetected = true;
        break;
      case RIGHT:
          xTile = (int)( ( (player->position.x + (player->hurtbox.width / 2) ) / map->tile_width) );
          yTile = (int)( ( (player->position.y) / map->tile_height) );
          printf( "X Tile: %d\n", xTile );
          printf( "Y Tile: %d\n", yTile );

          if( xTile <= 0 )
            xTile = 0;
          else if( xTile >= map->width - 1 )
            xTile = map->width - 1;
          if( yTile <= 0 )
            yTile = 0;
          else if( yTile >= map->height - 1)
            yTile = map->height - 1;

          gid = getGID(layerHead, map->width, xTile, yTile);
          tile = map->tiles[gid];
          if( gid && tile->collision )
            collisionDetected = true;
        break;
      case UP:
          xTile = (int)( ( (player->position.x) / map->tile_width) );
          yTile = (int)( ( (player->position.y - (player->hurtbox.height/ 2) - 1) / map->tile_height) );
          printf( "X Tile: %d\n", xTile );
          printf( "Y Tile: %d\n", yTile );

          if( xTile <= 0 )
            xTile = 0;
          else if( xTile >= map->width - 1 )
            xTile = map->width - 1;
          if( yTile <= 0 )
            yTile = 0;
          else if( yTile >= map->height - 1)
            yTile = map->height - 1;

          gid = getGID(layerHead, map->width, xTile, yTile);
          tile = map->tiles[gid];
          if( gid && tile->collision )
            collisionDetected = true;
        break;
      case DOWN:
          xTile = (int)( ( (player->position.x) / map->tile_width) );
          yTile = (int)( ( (player->position.y + (player->hurtbox.height/ 2) ) / map->tile_height) );
          printf( "X Tile: %d\n", xTile );
          printf( "Y Tile: %d\n", yTile );

          if( xTile <= 0 )
            xTile = 0;
          else if( xTile >= map->width - 1 )
            xTile = map->width - 1;
          if( yTile <= 0 )
            yTile = 0;
          else if( yTile >= map->height - 1)
            yTile = map->height - 1;

          gid = getGID(layerHead, map->width, xTile, yTile);
          tile = map->tiles[gid];
          if( gid && tile->collision )
            collisionDetected = true;
        break;
    }
    layerHead = layerHead->next;
  }

  if( collisionDetected )
    printf( "Collision Detected\n" );

  return collisionDetected;
}

int32_t getGID( tmx_layer *layer, unsigned int mapWidth, unsigned int column, unsigned int row )
{
  return layer->content.gids[ row * mapWidth + column ];
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
